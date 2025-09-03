/*---------------------------------------------------------*\
| ConfigManage.cpp                                          |
|                                                           |
|   Minimal configuration manager for Skydimo helpers       |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ConfigManage.h"

ConfigManager & ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

int ConfigManager::loadInstallDirection(const QString & /*device_id*/)
{
    // Default to normal orientation when no config backend is present.
    return 0;
}

void ConfigManager::updateInstallDirection(const QString & /*device_id*/, int /*direction*/)
{
    // No-op placeholder until integrated with real settings backend.
}


