/*---------------------------------------------------------*\
| RGBController_SkydimoHID.cpp                              |
|                                                           |
|   RGBController for Skydimo HID LED Strip                 |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SkydimoHID.h"
#include "SkydimoDeviceConfig.h"
#include "SkydimoZones.h"
#include "SkydimoMapping.h"
extern "C" bool Skydimo_IsDetectionGuardEnabled();

/**
 * @brief Constructor.
 * @param controller_ptr A unique_ptr to the hardware controller.
 * @details Initializes controller information and modes.
 */
RGBController_SkydimoHID::RGBController_SkydimoHID(std::unique_ptr<SkydimoHIDController> controller_ptr)
    : controller(std::move(controller_ptr))
{
    // Set device information from the hardware controller
    name                = controller->GetDeviceName();
    vendor              = "Skydimo";
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "Skydimo HID Device";
    version             = "1.0";
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    // Create Direct control mode
    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    // Removed legacy Off mode for Skydimo devices

    SetupZones();
}

/**
 * @brief Destructor.
 * @details The unique_ptr will automatically handle hardware controller cleanup.
 */
RGBController_SkydimoHID::~RGBController_SkydimoHID()
{
    // 释放为矩阵区域分配的内存
    for(size_t zi = 0; zi < zones.size(); zi++)
    {
        zone &z = zones[zi];
        if(z.matrix_map)
        {
            if(z.matrix_map->map)
            {
                delete[] z.matrix_map->map;
                z.matrix_map->map = nullptr;
            }
            delete z.matrix_map;
            z.matrix_map = nullptr;
        }
    }
}

/**
 * @brief Sets up the device zones.
 * @details Creates zones based on the device model configuration.
 *          Falls back to a single zone if model is not recognized.
 */
void RGBController_SkydimoHID::SetupZones()
{
    SkydimoZones::setupZonesForDeviceName(name, zones, leds);

    SetupColors();

    // 同步 zones 的实际 LED 数量到底层 HID 控制器
    if(!zones.empty())
    {
        int total_leds = 0;
        for(const zone &z : zones)
        {
            total_leds += static_cast<int>(z.leds_count);
        }
        controller->SetLEDCount(total_leds);
    }
}

void RGBController_SkydimoHID::SetInstallDirection(SkydimoInstallDirection direction)
{
    m_installDirection = direction;
}

/**
 * @brief Resizes a zone (the number of LEDs).
 * @param zone The index of the zone to resize.
 * @param new_size The new number of LEDs.
 * @details Model-specific configurations do not support resizing.
 *          Only unknown models with a single zone can be resized.
 */
void RGBController_SkydimoHID::ResizeZone(int zone, int new_size)
{
    if(zone >= static_cast<int>(zones.size()))
    {
        return;
    }

    // Check if this is a model-specific configuration
    std::string model_id = ExtractModelFromDeviceName(name);
    if (GetSkydimoModelConfig(model_id) != nullptr)
    {
        // Model-specific configurations have fixed zone sizes
        return;
    }

    // Only allow resizing for unknown models with single zone
    if(new_size < zones[zone].leds_min || new_size > zones[zone].leds_max)
    {
        return;
    }

    // 只有当LED数量真正改变时，才在重新分配LED之前将所有LED设置为黑色（熄灭）
    // 这样可以确保当LED数量减少时，多余的LED会被熄灭，同时避免不必要的闪烁
    if(zones[zone].leds_count != static_cast<unsigned int>(new_size) && !colors.empty())
    {
        // 将所有现有LED设置为黑色
        for(size_t i = 0; i < colors.size(); i++)
        {
            colors[i] = 0x00000000;  // 黑色
        }
        
        // 立即更新硬件以熄灭所有LED
        DeviceUpdateLEDs();
    }

    zones[zone].leds_count = new_size;

    // Re-create the LED list
    leds.clear();
    for(int i = 0; i < new_size; i++)
    {
        led new_led;
        new_led.name = "LED " + std::to_string(i + 1);
        leds.push_back(new_led);
    }

    SetupColors();

    // 同步 zones 的实际 LED 数量到底层 HID 控制器
    int total_leds = 0;
    for(size_t zi = 0; zi < zones.size(); zi++)
    {
        total_leds += (int)zones[zi].leds_count;
    }
    controller->SetLEDCount(total_leds);
}

/**
 * @brief Updates all LEDs on the device.
 * @details Sends the color array to the hardware.
 */
void RGBController_SkydimoHID::DeviceUpdateLEDs()
{
    // 在 OpenRGB 原生清理/重扫阶段，避免向已关闭的 HID 句柄写入
    if(Skydimo_IsDetectionGuardEnabled())
    {
        return;
    }
    // 简化策略：若为矩阵合并设备，则根据安装方向对发送序列进行重排
    if(!zones.empty() && zones[0].type == ZONE_TYPE_MATRIX && zones[0].matrix_map != nullptr)
    {
        SkydimoMapping::reorderColorsForMatrix(zones[0].matrix_map, colors, m_installDirection, m_sendBuffer);
        controller->SetLEDs(m_sendBuffer);
        return;
    }

    // 非矩阵或未知模型，直接发送
    controller->SetLEDs(colors);
}

/**
 * @brief Updates the LEDs for a specific zone.
 * @param zone The index of the zone to update.
 * @details This device requires a full update, so it updates all LEDs.
 */
void RGBController_SkydimoHID::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

/**
 * @brief Updates a single LED.
 * @param led The index of the LED to update.
 * @note This device requires a full update, so it calls DeviceUpdateLEDs.
 */
void RGBController_SkydimoHID::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

/**
 * @brief Updates the device mode.
 * @details Handles mode changes: turns LEDs off for "Off" mode.
 */
void RGBController_SkydimoHID::DeviceUpdateMode()
{
    // Off mode removed; Direct mode requires no special handling here.
}
