/*---------------------------------------------------------*\
| i2c_smbus_pawnio.cpp                                      |
|                                                           |
|   PawnIO SMBus driver implementation for Windows          |
|                                                           |
|   Ported from upstream OpenRGB (May 2025)                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "Detector.h"
#include "i2c_smbus_pawnio.h"
#include "LogManager.h"
#include "PawnIOLib.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "wmi.h"

std::unordered_map<std::string, int> i2c_smbus_pawnio::using_handle;

i2c_smbus_pawnio::i2c_smbus_pawnio(HANDLE handle, std::string name)
{
    this->handle    = handle;
    this->name      = name;

    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

    bool shared_smbus_access = true;

    if(drivers_settings.contains("shared_smbus_access"))
    {
        shared_smbus_access = drivers_settings["shared_smbus_access"].get<bool>();
    }

    if(shared_smbus_access)
    {
        global_smbus_access_handle = CreateMutexA(NULL, FALSE, GLOBAL_SMBUS_MUTEX_NAME);
    }

    using_handle[name]++;
}

i2c_smbus_pawnio::~i2c_smbus_pawnio()
{
    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }

    if(--using_handle[name] == 0 && pawnio_close(handle))
    {
        LOG_ERROR("PawnIO failed to close");
    }
}

s32 i2c_smbus_pawnio::pawnio_read(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
    const SIZE_T    in_size         = 8;
    ULONG64         in[in_size];
    const SIZE_T    out_size        = 9;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    in[0]   = addr;
    in[1]   = read_write;
    in[2]   = command;
    in[3]   = size;

    if(data != NULL)
    {
        memcpy(&in[4], data, sizeof(i2c_smbus_data));
    }

    status  = pawnio_execute(handle, "ioctl_smbus_read", in, in_size, out, out_size, &return_size);

    if(data != NULL)
    {
        memcpy(data, &out[0], sizeof(i2c_smbus_data));
    }

    return(status);
}

s32 i2c_smbus_pawnio::pawnio_write(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
    const SIZE_T    in_size         = 8;
    ULONG64         in[in_size];
    SIZE_T          return_size;

    in[0]   = addr;
    in[1]   = read_write;
    in[2]   = command;
    in[3]   = size;

    if(data != NULL)
    {
        memcpy(&in[4], data, sizeof(i2c_smbus_data));
    }

    return(pawnio_execute(handle, "ioctl_smbus_write", in, in_size, NULL, 0, &return_size));
}

s32 i2c_smbus_pawnio::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }

    s32 status = pawnio_read(addr, read_write, command, size, data);

    if(global_smbus_access_handle != NULL)
    {
        ReleaseMutex(global_smbus_access_handle);
    }

    return(status);
}

s32 i2c_smbus_pawnio::i2c_xfer(u8 addr, char read_write, int* size, u8* data)
{
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }

    const SIZE_T    in_size         = 8;
    ULONG64         in[in_size];
    const SIZE_T    out_size        = 5;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    in[0]   = addr;
    in[1]   = read_write;
    in[2]   = 0;
    in[3]   = 0;

    if(size != NULL && data != NULL)
    {
        in[4]   = *size;
        memcpy(&in[5], data, sizeof(i2c_smbus_data));
    }

    status = pawnio_execute(handle, "ioctl_i2c_xfer", in, in_size, out, out_size, &return_size);

    if(size != NULL && data != NULL)
    {
        *size = static_cast<int>(out[0]);
        memcpy(data, &out[1], sizeof(i2c_smbus_data));
    }

    if(global_smbus_access_handle != NULL)
    {
        ReleaseMutex(global_smbus_access_handle);
    }

    return(status);
}

HRESULT i2c_smbus_pawnio::start_pawnio(std::string filename, PHANDLE phandle)
{
    char    exePath[MAX_PATH];
    HANDLE  handle;
    HRESULT status;

    LOG_INFO("Start PawnIO: %s", filename.c_str());

    if(!GetModuleFileNameA(NULL, exePath, MAX_PATH))
    {
        LOG_ERROR("Failed to get executable path, PawnIO initialization aborted");
        return E_FAIL;
    }

    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path filePath = exeDir / filename;

    if(!std::filesystem::exists(filePath))
    {
        LOG_ERROR("Failed to find %s in the executable's directory, PawnIO initialization aborted", filename.c_str());
        return(E_FAIL);
    }

    std::ifstream file(filePath, std::ios::binary);

    if(!file.is_open())
    {
        LOG_ERROR("Failed to open %s, PawnIO initialization aborted", filename.c_str());
        return(E_FAIL);
    }

    std::vector<char> blob((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    status = pawnio_open(phandle);

    if(status)
    {
        if(status == E_ACCESSDENIED)
        {
            LOG_ERROR("Permission Denied, PawnIO initialization aborted");
        }
        else
        {
            LOG_ERROR("Could not open PawnIO, PawnIO initialization aborted");
        }

        return(status);
    }

    handle = *phandle;

    status = pawnio_load(handle, reinterpret_cast<const UCHAR*>(blob.data()), blob.size());

    if(status)
    {
        pawnio_close(handle);

        if (status == HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED))
        {
            LOG_INFO("PawnIO module initialization aborted (unsupported)");
        }
        else
        {
            LOG_ERROR("PawnIO module initialization aborted (code=%ld)", status);
        }

        return(status);
    }

    LOG_INFO("PawnIO initialized successully");
    return(S_OK);
}

s32 piix4_port_sel(HANDLE pawnio_handle, s32 port)
{
    const SIZE_T    in_size         = 1;
    ULONG64         in[in_size]     = {(ULONG64)port};
    const SIZE_T    out_size        = 1;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_piix4_port_sel", in, in_size, out, 1, &return_size);

    return(status ? -EIO : 0);
}

bool i2c_smbus_pawnio_detect()
{
    ULONG dll_version;
    if(pawnio_version(&dll_version))
    {
        LOG_INFO("PawnIO is not loaded, PawnIO I2C bus detection aborted");
        return(false);
    }

    i2c_smbus_interface *   bus;
    HANDLE                  pawnio_handle;

    if(i2c_smbus_pawnio::start_pawnio("SmbusI801.bin", &pawnio_handle) == S_OK)
    {
        bus = new i2c_smbus_pawnio(pawnio_handle, "i801");
        ResourceManager::get()->RegisterI2CBus(bus);
    }

    if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) == S_OK)
    {
        piix4_port_sel(pawnio_handle, 0);

        bus = new i2c_smbus_pawnio(pawnio_handle, "piix4");
        ResourceManager::get()->RegisterI2CBus(bus);
    }

    if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) == S_OK)
    {
        piix4_port_sel(pawnio_handle, 1);

        bus = new i2c_smbus_pawnio(pawnio_handle, "piix4");
        ResourceManager::get()->RegisterI2CBus(bus);
    }

    if(i2c_smbus_pawnio::start_pawnio("SmbusNCT6793.bin", &pawnio_handle) == S_OK)
    {
        bus = new i2c_smbus_pawnio(pawnio_handle, "NCT6793");
        ResourceManager::get()->RegisterI2CBus(bus);
    }

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_pawnio_detect);

