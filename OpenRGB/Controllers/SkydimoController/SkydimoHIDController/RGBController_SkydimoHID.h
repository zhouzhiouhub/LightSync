/*---------------------------------------------------------*\
| RGBController_SkydimoHID.h                                |
|                                                           |
|   RGBController for the Skydimo HID LED Strip.            |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project.               |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkydimoHIDController.h"
#include "SkydimoInstallDirectionManager.h"
#include "SkydimoDeviceConfig.h"
#include <memory>

/**
 * @brief OpenRGB controller class for the Skydimo HID LED strip.
 * @details This class inherits from RGBController and implements the
 *          OpenRGB interface for the device.
 */
class RGBController_SkydimoHID : public RGBController
{
public:
    explicit RGBController_SkydimoHID(std::unique_ptr<SkydimoHIDController> controller_ptr);
    ~RGBController_SkydimoHID();

    /**
     * @brief Sets up the device zones.
     * @details Creates a single zone representing the LED strip with a variable
     *          number of LEDs.
     */
    void SetupZones();

    /**
     * @brief Resizes a zone (the number of LEDs).
     * @param zone The index of the zone to resize (always 0).
     * @param new_size The new number of LEDs.
     * @note This device supports resizing the LED count.
     */
    void ResizeZone(int zone, int new_size);

    /**
     * @brief Updates all LEDs on the device.
     * @details Sends the colors of all LEDs to the device.
     */
    void DeviceUpdateLEDs();

    /**
     * @brief Updates the LEDs for a specific zone.
     * @param zone The index of the zone to update.
     */
    void UpdateZoneLEDs(int zone);

    /**
     * @brief Updates a single LED.
     * @param led The index of the LED to update.
     * @note This device requires a full update for all LEDs.
     */
    void UpdateSingleLED(int led);

    /**
     * @brief Updates the device mode.
     * @details This device only supports the "Direct" control mode.
     */
    void DeviceUpdateMode();

    // 安装方向设置（简单精简实现）：仅在更新LED时按方向重排颜色
    void SetInstallDirection(SkydimoInstallDirection direction);

    // 适配 ControlPage 旧调用：默认返回 false（未自定义配置）
    bool IsUserConfigured() const { return false; }
    // 适配 ControlPage 旧调用：空实现
    void ResetToDefaultConfig() {}
    // 适配 ControlPage 旧调用：返回一个最小结构（仅告知不可编辑）
    struct MinimalDeviceConfig { bool is_editable = false; };
    MinimalDeviceConfig GetDeviceConfig() const {
        // 检查是否为未知型号（允许编辑LED数量）
        std::string model_id = ExtractModelFromDeviceName(name);
        MinimalDeviceConfig config;
        config.is_editable = (GetSkydimoModelConfig(model_id) == nullptr);
        return config;
    }

private:
    std::unique_ptr<SkydimoHIDController> controller;    ///< Pointer to the hardware controller

    SkydimoInstallDirection m_installDirection = INSTALL_NORMAL;
    std::vector<RGBColor> m_sendBuffer; // 复用发送缓冲，避免重复分配
};
