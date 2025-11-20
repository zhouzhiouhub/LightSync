/*---------------------------------------------------------*\
| RGBController_X87Keyboard.h                               |
|                                                           |
|   RGBController wrapper for the X87 keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "X87KeyboardController.h"

class RGBController_X87Keyboard : public RGBController
{
public:
    explicit RGBController_X87Keyboard(X87KeyboardController* controller_ptr);
    ~RGBController_X87Keyboard() override;

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;
    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;
    void DeviceUpdateMode() override;
    void DeviceSaveMode() override;

private:
    X87KeyboardController* controller;
};

