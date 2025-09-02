/*---------------------------------------------------------*\
| SkydimoInstallDirectionManager.h                          |
|                                                           |
|   Install direction (orientation) helpers for Skydimo     |
|   2/3/4-zone devices that are represented as a matrix.    |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <QString>
#include "ConfigManage.h"

enum SkydimoInstallDirection
{
    INSTALL_NORMAL = 0,          // 左至右 + 上至下
    INSTALL_FLIP_HORIZONTAL = 1, // 右至左 + 上至下
    INSTALL_FLIP_VERTICAL = 2,   // 左至右 + 下至上
    INSTALL_FLIP_BOTH = 3        // 右至左 + 下至上
};

struct SkydimoInstallDirectionManager
{
    static const char* DirectionToString(SkydimoInstallDirection dir)
    {
        switch(dir)
        {
            case INSTALL_NORMAL:          return "Normal (LTR+TTB)";   // 左至右 + 上至下
            case INSTALL_FLIP_HORIZONTAL: return "Flip Horizontal";    // 水平翻转
            case INSTALL_FLIP_VERTICAL:   return "Flip Vertical";      // 垂直翻转
            case INSTALL_FLIP_BOTH:       return "Flip Both";          // 全翻转
        }
        return "Unknown";
    }

    // 简易检测占位：可按 location/name 从配置或规则推断，默认正常方向
    static SkydimoInstallDirection DetectInstallDirectionByDeviceId(const std::string& device_id)
    {
        ConfigManager &cm = ConfigManager::getInstance();
        int value = cm.loadInstallDirection(QString::fromStdString(device_id));
        if(value < static_cast<int>(INSTALL_NORMAL) || value > static_cast<int>(INSTALL_FLIP_BOTH))
        {
            value = static_cast<int>(INSTALL_NORMAL);
        }
        return static_cast<SkydimoInstallDirection>(value);
    }
};

// 占位的坐标映射/配置存储器。后续可接入实际配置系统。
struct SkydimoCoordinateMapper
{
    static void SaveInstallDirectionToConfig(const std::string& device_id, SkydimoInstallDirection dir)
    {
        ConfigManager &cm = ConfigManager::getInstance();
        cm.updateInstallDirection(QString::fromStdString(device_id), static_cast<int>(dir));
    }
};

inline std::string GetCacheStatistics()
{
    return std::string("N/A");
}



