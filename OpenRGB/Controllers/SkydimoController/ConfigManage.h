/*---------------------------------------------------------*\
| ConfigManage.h                                            |
|                                                           |
|   Minimal configuration manager for Skydimo helpers       |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QString>

class ConfigManager
{
public:
    static ConfigManager & getInstance();

    int loadInstallDirection(const QString &device_id);

    void updateInstallDirection(const QString &device_id, int direction);

private:
    ConfigManager() {}
    ConfigManager(const ConfigManager &) = delete;
    ConfigManager & operator=(const ConfigManager &) = delete;
};


