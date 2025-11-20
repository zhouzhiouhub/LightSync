/*---------------------------------------------------------*\
| X87KeyboardControllerDetect.cpp                           |
|                                                           |
|   Device detection for the X87 keyboard                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include <string>

#include "Detector.h"
#include "ResourceManager.h"
#include "X87KeyboardController.h"
#include "RGBController_X87Keyboard.h"

namespace
{
    constexpr unsigned short X87_KEYBOARD_VID        = 0x0C45;
    constexpr unsigned short X87_KEYBOARD_PID        = 0x8006;
    constexpr int            X87_KEYBOARD_INTERFACE  = 2;
}

void DetectX87Keyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev == nullptr)
    {
        return;
    }

    auto* controller = new X87KeyboardController(dev, info->path, name);
    auto* rgb        = new RGBController_X87Keyboard(controller);

    ResourceManager::get()->RegisterRGBController(rgb);
}

REGISTER_HID_DETECTOR_I("EWEADN X87 Keyboard", DetectX87Keyboards, X87_KEYBOARD_VID, X87_KEYBOARD_PID, X87_KEYBOARD_INTERFACE);

