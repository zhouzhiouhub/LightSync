/*---------------------------------------------------------*\
| SkydimoHIDController.cpp                                  |
|                                                           |
|   Driver for Skydimo HID LED Strip                        |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SkydimoHIDController.h"
#include <hidapi.h>
#include "LogManager.h"
#include <vector>
#include <thread>
#include <array>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <sstream>

extern "C" bool Skydimo_IsDetectionGuardEnabled();

/**
 * @brief Constructor.
 * @details Initializes member variables.
 */
SkydimoHIDController::SkydimoHIDController()
{
    device          = nullptr;
    device_path     = "";
    device_name     = "Skydimo LED Strip";
    device_serial   = "000000";
    vid             = VID;
    pid             = PID;
    max_leds        = DEFAULT_MAX_LEDS;
    num_leds        = DEFAULT_MAX_LEDS;
    last_update_time = std::chrono::steady_clock::now();
}

/**
 * @brief Destructor.
 * @details Cleans up HID device resources.
 */
SkydimoHIDController::~SkydimoHIDController()
{
    CloseDevice();
}

/**
 * @brief Initializes the controller and opens the HID device.
 * @param path The system path of the HID device.
 * @return true on success, false on failure.
 */
bool SkydimoHIDController::Initialize(const std::string& path)
{
    device_path = path;

    device = hid_open_path(path.c_str());
    if(device == nullptr)
    {
        return false;
    }

    // Fetch device information
    wchar_t wstr[256];

    // Get product name
    if(hid_get_product_string(device, wstr, sizeof(wstr) / sizeof(wstr[0])) == 0)
    {
        // Convert wide string to multibyte string
        char product_name[256];
        wcstombs(product_name, wstr, sizeof(product_name));
        if(strlen(product_name) > 0)
        {
            device_name = "Skydimo " + std::string(product_name);
        }
    }

    // Get serial number
    if(hid_get_serial_number_string(device, wstr, sizeof(wstr) / sizeof(wstr[0])) == 0)
    {
        // Convert wide char serial to hex string
        std::ostringstream oss;
        oss << std::uppercase << std::hex << std::setfill('0');
        for(int i = 0; wstr[i] != 0 && i < 16; i++)
        {
            oss << std::setw(2) << static_cast<int>(wstr[i] & 0xFF);
        }

        std::string serial_hex = oss.str();
        if(!serial_hex.empty())
        {
            device_serial = serial_hex;
        }
    }

    return true;
}

/**
 * @brief Closes the HID device.
 */
void SkydimoHIDController::CloseDevice()
{
    std::lock_guard<std::mutex> lk(device_mutex);
    closing.store(true);
    if(device)
    {
        hid_close(device);
        device = nullptr;
    }
}

/**
 * @brief Sets the colors for the LEDs.
 * @param colors A vector of RGBColor structs.
 * @param count The number of LEDs to update.
 * @return true on success, false on failure.
 */
bool SkydimoHIDController::SetLEDs(const std::vector<RGBColor>& colors)
{
    if(colors.empty())
    {
        return false;
    }

    if(closing.load() || Skydimo_IsDetectionGuardEnabled())
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Enforce update rate to prevent HID flooding          |
    \*-----------------------------------------------------*/
    EnforceUpdateRate();

    // Clamp LED count to a reasonable range
    int led_count = std::min(num_leds, std::min(static_cast<int>(colors.size()), max_leds));

    // Send LED data in batches
    // SKLOG_DEBUG(devLog) << "[HID] SetLEDs begin, led_count=" << led_count << ", num_leds=" << num_leds;
    for(int idx = 0; idx < led_count; idx += BATCH_LEDS)
    {
        if(closing.load() || Skydimo_IsDetectionGuardEnabled())
        {
            return false;
        }
        int current_batch_size = std::min(BATCH_LEDS, led_count - idx);

        // Prepare RGB data (in GRB order)
        std::array<uint8_t, MAX_RGB_BYTES> rgb_data_bytes{};
        for(int i = 0; i < current_batch_size; i++)
        {
            RGBColor color = colors[idx + i];
            uint8_t r = RGBGetRValue(color);
            uint8_t g = RGBGetGValue(color);
            uint8_t b = RGBGetBValue(color);

            // Device uses GRB order
            rgb_data_bytes[i * 3]     = g;
            rgb_data_bytes[i * 3 + 1] = r;
            rgb_data_bytes[i * 3 + 2] = b;
        }

        // Send the current batch of RGB data
        if(!SendRGBData(rgb_data_bytes.data(), idx))
        {
            LOG_ERROR("[HID] SendRGBData failed at offset %d", idx);
            return false;
        }
    }

    // Send the end command
    if(closing.load() || Skydimo_IsDetectionGuardEnabled())
    {
        return false;
    }

    bool ok = SendEndCommand(led_count);
    if(!ok)
    {
        LOG_ERROR("[HID] SendEndCommand failed, total_leds=%d", led_count);
    }
    return ok;
}

void SkydimoHIDController::SetLEDCount(int count)
{
    if(count < 0)
    {
        count = 0;
    }
    num_leds = count;
}

/**
 * @brief Calculates the CRC8 checksum (MAXIM polynomial).
 * @param data Pointer to the data buffer.
 * @param size Size of the data.
 * @return The calculated CRC8 checksum.
 */
uint8_t SkydimoHIDController::CalculateCRC8(const uint8_t* data, uint8_t size) const
{
    #define SKYDIMO_CRC8_POLY 0x07
    uint8_t crc = 0x00;

    for(uint8_t i = 0; i < size; i++)
    {
        crc ^= data[i];
        for(int j = 0; j < 8; j++)
        {
            if(crc & 0x80)
            {
                crc = static_cast<uint8_t>(((crc << 1) & 0xFF) ^ SKYDIMO_CRC8_POLY);
            }
            else
            {
                crc = static_cast<uint8_t>((crc << 1) & 0xFF);
            }
        }
    }

    return static_cast<uint8_t>(crc & 0xFF);
}

/**
 * @brief Sends RGB data to the device.
 * @param rgb_data Pointer to the buffer of RGB data.
 * @param offset The starting offset of the LEDs.
 * @return true on success, false on failure.
 */
bool SkydimoHIDController::SendRGBData(const uint8_t* rgb_data, int offset)
{
    // 仅使用经过验证的 64 字节输出报告（无 Report ID）
    const uint8_t pre_crc_length = 63; // 1(cmd)+2(offset)+60(data)
    std::vector<uint8_t> packet;
    packet.reserve(64);
    packet.push_back(0x01);
    packet.push_back((uint8_t)(offset & 0xFF));
    packet.push_back((uint8_t)((offset >> 8) & 0xFF));
    packet.insert(packet.end(), rgb_data, rgb_data + MAX_RGB_BYTES);
    packet.push_back(CalculateCRC8(packet.data(), pre_crc_length));

    int result = hid_write(device, packet.data(), (int)packet.size());
    if(result >= 0)
    {
        // SKLOG_DEBUG(devLog) << "[HID] SendRGBData ok, legacy-64, offset=" << offset << ", bytes=" << (int)packet.size();
        return true;
    }
    // SKLOG_ERROR(devLog) << "[HID] SendRGBData failed, legacy-64, offset=" << offset;
    return false;
}

/**
 * @brief Sends the end command to finalize the color update.
 * @param total_leds The total number of LEDs being updated.
 * @return true on success, false on failure.
 */
bool SkydimoHIDController::SendEndCommand(int total_leds)
{
    // 仅使用经过验证的 64 字节输出报告（无 Report ID）
    const int pre_crc_length = 63;
    std::vector<uint8_t> end_payload;
    end_payload.reserve(64);
    end_payload.push_back(0x01);
    end_payload.push_back(0xFF);
    end_payload.push_back(0xFF);
    end_payload.push_back((uint8_t)(total_leds & 0xFF));
    end_payload.push_back((uint8_t)((total_leds >> 8) & 0xFF));
    if((int)end_payload.size() < pre_crc_length)
    {
        end_payload.resize(pre_crc_length, 0x00);
    }
    end_payload.push_back(CalculateCRC8(end_payload.data(), (uint8_t)pre_crc_length));

    {
        std::lock_guard<std::mutex> lk(device_mutex);
        if(!device || closing.load())
        {
            return false;
        }
        int result = hid_write(device, end_payload.data(), (int)end_payload.size());
        if(result >= 0)
        {
            return true;
        }
    }
    LOG_ERROR("[HID] SendEndCommand failed, legacy-64, total_leds=%d", total_leds);
    return false;
}

void SkydimoHIDController::EnforceUpdateRate()
{
    /*-----------------------------------------------------*\
    | Limit update rate to 60 FPS to prevent HID flooding  |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::milliseconds time_since_last_update =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time);

    if(time_since_last_update.count() < 16)  // 60 FPS = ~16ms per frame
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16 - time_since_last_update.count()));
    }

    last_update_time = std::chrono::steady_clock::now();
}
