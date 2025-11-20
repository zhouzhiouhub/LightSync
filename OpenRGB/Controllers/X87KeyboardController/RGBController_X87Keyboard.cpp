/*---------------------------------------------------------*\
| RGBController_X87Keyboard.cpp                             |
|                                                           |
|   RGBController wrapper for the X87 keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_X87Keyboard.h"

namespace
{
    constexpr unsigned int NA = 0xFFFFFFFF;

    unsigned int x87_keyboard_matrix[6][24] =
    {
        {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  78,  83,  NA,  NA,  NA,  NA,  NA },
        {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  79,  84,  NA,  87,  92,  96, 101 },
        {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
        {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
        {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  81,  NA,  NA,  90,  95,  99, 103 },
        {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  82,  86,  NA,  91,  NA, 100,  NA }
    };
}

RGBController_X87Keyboard::RGBController_X87Keyboard(X87KeyboardController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetNameString();
    vendor          = "EWEADN";
    type            = DEVICE_TYPE_KEYBOARD;
    description     = "EWEADN X87 Keyboard Device";
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_X87Keyboard::~RGBController_X87Keyboard()
{
    for(zone& z : zones)
    {
        if(z.matrix_map != nullptr)
        {
            delete z.matrix_map;
            z.matrix_map = nullptr;
        }
    }

    delete controller;
    controller = nullptr;
}

void RGBController_X87Keyboard::SetupZones()
{
    zone keyboard_zone;

    keyboard_zone.name           = "Keyboard";
    keyboard_zone.type           = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min       = X87KeyboardController::LED_COUNT;
    keyboard_zone.leds_max       = X87KeyboardController::LED_COUNT;
    keyboard_zone.leds_count     = X87KeyboardController::LED_COUNT;
    keyboard_zone.matrix_map     = new matrix_map_type;
    keyboard_zone.matrix_map->height = 6;
    keyboard_zone.matrix_map->width  = 24;
    keyboard_zone.matrix_map->map    = &x87_keyboard_matrix[0][0];

    zones.push_back(keyboard_zone);

    for(unsigned int led_index = 0; led_index < X87KeyboardController::LED_COUNT; ++led_index)
    {
        led keyboard_led;
        keyboard_led.name = "Keyboard LED " + std::to_string(led_index);
        leds.push_back(keyboard_led);
    }

    SetupColors();
}

void RGBController_X87Keyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Not supported
}

void RGBController_X87Keyboard::DeviceUpdateLEDs()
{
    controller->SetKeyboardColors(colors);
}

void RGBController_X87Keyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_X87Keyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_X87Keyboard::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_X87Keyboard::DeviceSaveMode()
{
    // Nothing to persist
}

