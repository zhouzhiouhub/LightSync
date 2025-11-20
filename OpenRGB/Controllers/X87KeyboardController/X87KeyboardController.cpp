/*---------------------------------------------------------*\
| X87KeyboardController.cpp                                 |
|                                                           |
|   Hardware interface for the X87 keyboard                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "X87KeyboardController.h"

#include <cstring>
#include <iterator>

#include "LogManager.h"
#include "StringUtils.h"

namespace
{
    constexpr unsigned char REPORT_ID = 0x00;
    constexpr unsigned char REPORT_HEADER_0 = 0xAA;
    constexpr unsigned char REPORT_HEADER_1 = 0x24;
    constexpr unsigned char REPORT_HEADER_2 = 0x38;
}

X87KeyboardController::X87KeyboardController(hid_device* dev_handle, const char* path, const std::string& device_name)
    : dev(dev_handle)
    , location(path ? path : "")
    , name(device_name)
{
}

X87KeyboardController::~X87KeyboardController()
{
    if(dev != nullptr)
    {
        hid_close(dev);
        dev = nullptr;
    }
}

std::string X87KeyboardController::GetDeviceLocation() const
{
    return std::string("HID: ") + location;
}

std::string X87KeyboardController::GetNameString() const
{
    return name;
}

std::string X87KeyboardController::GetSerialString() const
{
    if(dev == nullptr)
    {
        return "";
    }

    wchar_t serial_string[128];
    const int result = hid_get_serial_number_string(dev, serial_string, static_cast<size_t>(std::size(serial_string)));

    if(result != 0)
    {
        return "";
    }

    return StringUtils::wstring_to_string(serial_string);
}

void X87KeyboardController::SetKeyboardColors(const std::vector<RGBColor>& colors)
{
    if(dev == nullptr)
    {
        return;
    }

    for(unsigned int page = 0; page < PAGE_COUNT; ++page)
    {
        unsigned char report_buffer[REPORT_SIZE];
        BuildPageReport(page, colors, report_buffer);
        SendReport(report_buffer);
    }
}

void X87KeyboardController::BuildPageReport(unsigned int page_index, const std::vector<RGBColor>& colors, unsigned char report[REPORT_SIZE]) const
{
    std::memset(report, 0x00, REPORT_SIZE);

    report[0] = REPORT_HEADER_0;
    report[1] = REPORT_HEADER_1;
    report[2] = REPORT_HEADER_2;

    const unsigned int page_offset = page_index * PAGE_STEP;
    report[3] = static_cast<unsigned char>(page_offset & 0xFF);
    report[4] = static_cast<unsigned char>((page_offset >> 8) & 0xFF);

    const unsigned int base_index = page_index * LEDS_PER_PAGE;

    for(unsigned int slot = 0; slot < LEDS_PER_PAGE; ++slot)
    {
        const unsigned int led_index = base_index + slot;
        const unsigned int payload_offset = PAYLOAD_OFFSET + (slot * ENTRY_SIZE);

        if(led_index >= LED_COUNT)
        {
            continue;
        }

        const RGBColor color = (led_index < colors.size()) ? colors[led_index] : 0;

        report[payload_offset + 0] = static_cast<unsigned char>(led_index & 0xFF);
        report[payload_offset + 1] = static_cast<unsigned char>(RGBGetRValue(color));
        report[payload_offset + 2] = static_cast<unsigned char>(RGBGetGValue(color));
        report[payload_offset + 3] = static_cast<unsigned char>(RGBGetBValue(color));
    }
}

void X87KeyboardController::SendReport(const unsigned char report[REPORT_SIZE])
{
    if(dev == nullptr)
    {
        return;
    }

    unsigned char output_buffer[REPORT_SIZE + 1];
    output_buffer[0] = REPORT_ID;
    std::memcpy(&output_buffer[1], report, REPORT_SIZE);

    const int result = hid_write(dev, output_buffer, static_cast<size_t>(std::size(output_buffer)));

    if(result < 0)
    {
        const wchar_t* error = hid_error(dev);

        if(error != nullptr)
        {
            LOG_ERROR("[X87Keyboard] HID write failed: %ls", error);
        }
        else
        {
            LOG_ERROR("[X87Keyboard] HID write failed with unknown error.");
        }
    }
}

