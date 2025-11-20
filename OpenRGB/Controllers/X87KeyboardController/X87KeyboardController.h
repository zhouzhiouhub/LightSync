/*---------------------------------------------------------*\
| X87KeyboardController.h                                   |
|                                                           |
|   Hardware interface for the X87 keyboard                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>

#include <hidapi.h>

#include "RGBController.h"

class X87KeyboardController
{
public:
    static constexpr unsigned int LED_COUNT      = 126;
    static constexpr unsigned int LEDS_PER_PAGE  = 14;

    X87KeyboardController(hid_device* dev_handle, const char* path, const std::string& device_name);
    ~X87KeyboardController();

    std::string GetDeviceLocation() const;
    std::string GetNameString() const;
    std::string GetSerialString() const;

    void        SetKeyboardColors(const std::vector<RGBColor>& colors);

private:
    static constexpr unsigned int REPORT_SIZE     = 64;
    static constexpr unsigned int PAGE_STEP       = 0x38;
    static constexpr unsigned int PAYLOAD_OFFSET  = 8;
    static constexpr unsigned int ENTRY_SIZE      = 4;
    static constexpr unsigned int PAGE_COUNT      = (LED_COUNT + LEDS_PER_PAGE - 1) / LEDS_PER_PAGE;

    void        BuildPageReport(unsigned int page_index, const std::vector<RGBColor>& colors, unsigned char report[REPORT_SIZE]) const;
    void        SendReport(const unsigned char report[REPORT_SIZE]);

    hid_device* dev;
    std::string location;
    std::string name;
};

