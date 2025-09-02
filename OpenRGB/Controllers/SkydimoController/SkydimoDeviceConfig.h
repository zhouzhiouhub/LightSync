/*---------------------------------------------------------*\
| SkydimoDeviceConfig.h                                     |
|                                                           |
|   Configuration for different Skydimo device models       |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include "RGBController.h"

/**
 * @brief Structure to define a zone configuration
 */
struct SkydimoZoneConfig
{
    std::string name;           ///< Zone name
    zone_type   type;          ///< Zone type (ZONE_TYPE_LINEAR, ZONE_TYPE_MATRIX, etc.)
    int         led_count;     ///< Number of LEDs in this zone
    int         led_start;     ///< Starting LED index for this zone
};

/**
 * @brief Structure to define a device model configuration
 */
struct SkydimoModelConfig
{
    std::vector<SkydimoZoneConfig>  zones;         ///< Zone configurations
    int                             total_leds;    ///< Total LED count across all zones
};

/**
 * @brief 为 2/3/4 区设备根据规则生成矩阵映射
 * 规则：
 * - 2 区：两个等长分区 → 两列并排、从下往上递增（左列先、右列后）
 * - 3/4 区：从左下角往上开始沿外边围成一个矩形，四个角留空；
 *   - 4 区：两对等长 → 取较小的一对作为竖边长度、较大的一对作为横边长度；
 *   - 3 区：找到相等的一对 s，令高度 h = s + 2；宽度 w 取满足环形可容纳全部 LED 的最小整数，使 2*(h-2)+2*(w-2) ≥ total。
 * 未匹配规则则返回 false。
 */
inline bool BuildSkydimoMatrixForModel(const SkydimoModelConfig &model,
                                       unsigned int &out_width,
                                       unsigned int &out_height,
                                       std::vector<unsigned int> &out_map)
{
    const size_t zone_count = model.zones.size();
    if(zone_count < 2 || zone_count > 4)
    {
        return false;
    }

    // 收集分区长度
    std::vector<int> lengths;
    lengths.reserve(zone_count);
    for(const auto &z : model.zones)
    {
        lengths.push_back(z.led_count);
    }


    // 2 区：两侧立边，Zone1 反向；矩阵横向按近似 16:9 比例留白填充
    if(zone_count == 2)
    {
        if(lengths[0] != lengths[1] || lengths[0] <= 0)
        {
            return false;
        }

        const int len1 = model.zones[0].led_count;
        const int len2 = model.zones[1].led_count;
        const int start1 = model.zones[0].led_start;
        const int start2 = model.zones[1].led_start;

        // 高度按左/右长度 + 2（留四角），宽度按近似 16:9 选取
        unsigned int height = static_cast<unsigned int>(len1) + 2u;
        if(height < 2u) height = 2u;
        unsigned int width = static_cast<unsigned int>(std::round((height) * (16.0 / 9.0)));
        if(width < 4u) width = 4u; // 至少能留四角

        out_width  = width;
        out_height = height;
        out_map.assign(width * height, 0xFFFFFFFFu);

        auto set_at = [&](unsigned int x, unsigned int y, unsigned int val)
        {
            if(x < width && y < height)
            {
                out_map[y * width + x] = val;
            }
        };

        const unsigned int inner_h = (height >= 2 ? height - 2u : 0u);

        // 左边（zone1），方向与 zone2 相反：zone2 自上到下，则此处自下到上。
        {
            const unsigned int x = 0u;
            const unsigned int max_len = std::min<unsigned int>(inner_h, static_cast<unsigned int>(len1));
            for(unsigned int i = 0; i < max_len; ++i)
            {
                unsigned int y = (height - 2u) - i; // 自下往上
                set_at(x, y, static_cast<unsigned int>(start1 + i));
            }
        }

        // 右边（zone2，正常顺序），自上到下
        {
            const unsigned int x = width - 1u;
            const unsigned int max_len = std::min<unsigned int>(inner_h, static_cast<unsigned int>(len2));
            for(unsigned int i = 0; i < max_len; ++i)
            {
                unsigned int y = 1u + i; // 自上到下
                set_at(x, y, static_cast<unsigned int>(start2 + i));
            }
        }

        return true;
    }

    // 3/4 区：矩形外框（四角留空），并将四边与原 zone 顺序绑定：
    // 顶部 = zone2，底部 = zone4（若存在），左侧 = zone1，右侧 = zone3。
    // 若上下或左右长度不等，则采用较大者确定矩形尺寸，较小一侧留下空位 (0xFFFFFFFF)。
    unsigned int height = 0;
    unsigned int width  = 0;

    // 读取各 zone 的长度与起始索引
    const int len1 = model.zones[0].led_count;
    const int len2 = model.zones.size() > 1 ? model.zones[1].led_count : 0;
    const int len3 = model.zones.size() > 2 ? model.zones[2].led_count : 0;
    const int len4 = model.zones.size() > 3 ? model.zones[3].led_count : 0;

    const int start1 = model.zones[0].led_start;
    const int start2 = model.zones.size() > 1 ? model.zones[1].led_start : 0;
    const int start3 = model.zones.size() > 2 ? model.zones[2].led_start : 0;
    const int start4 = model.zones.size() > 3 ? model.zones[3].led_start : 0;

    if(zone_count == 4)
    {
        height = static_cast<unsigned int>(std::max(len1, len3)) + 2u; // 左/右 边决定高度
        width  = static_cast<unsigned int>(std::max(len2, len4)) + 2u; // 上/下 边决定宽度
    }
    else // zone_count == 3
    {
        height = static_cast<unsigned int>(std::max(len1, len3)) + 2u; // 左/右 边决定高度
        width  = static_cast<unsigned int>(len2) + 2u;                 // 上边（zone2）决定宽度
        if(width < 4u) width = 4u; // 至少留四角
    }

    if(width < 2u || height < 2u)
    {
        return false;
    }

    // 生成外框映射（矩阵坐标采用 y=0 顶部、y=height-1 底部）：
    // 将 zone1/2/3/4 分别铺到 左/上/右/下 四边，未覆盖位置保持 0xFFFFFFFF。
    out_width  = width;
    out_height = height;
    out_map.assign(width * height, 0xFFFFFFFFu);

    auto set_at = [&](unsigned int x, unsigned int y, unsigned int val)
    {
        if(x < width && y < height)
        {
            out_map[y * width + x] = val;
        }
    };

    const unsigned int inner_w = (width  >= 2 ? width  - 2u : 0u);
    const unsigned int inner_h = (height >= 2 ? height - 2u : 0u);

    // 左边（zone1，自下往上）
    {
        const unsigned int x = 0u;
        const unsigned int max_len = std::min<unsigned int>(inner_h, static_cast<unsigned int>(len1));
        for(unsigned int i = 0; i < max_len; ++i)
        {
            unsigned int y = (height - 2u) - i; // 从底内格向上
            set_at(x, y, static_cast<unsigned int>(start1 + i));
        }
    }

    // 上边（zone2，自左往右）
    {
        const unsigned int y = 0u;
        const unsigned int max_len = std::min<unsigned int>(inner_w, static_cast<unsigned int>(len2));
        for(unsigned int i = 0; i < max_len; ++i)
        {
            unsigned int x = 1u + i;
            set_at(x, y, static_cast<unsigned int>(start2 + i));
        }
    }

    // 右边（zone3，自上往下）
    {
        const unsigned int x = width - 1u;
        const unsigned int max_len = std::min<unsigned int>(inner_h, static_cast<unsigned int>(len3));
        for(unsigned int i = 0; i < max_len; ++i)
        {
            unsigned int y = 1u + i;
            set_at(x, y, static_cast<unsigned int>(start3 + i));
        }
    }

    // 下边（zone4，自右往左，仅 4 区存在）
    if(zone_count == 4)
    {
        const unsigned int y = height - 1u;
        const unsigned int max_len = std::min<unsigned int>(inner_w, static_cast<unsigned int>(len4));
        for(unsigned int i = 0; i < max_len; ++i)
        {
            unsigned int x = (width - 2u) - i;
            set_at(x, y, static_cast<unsigned int>(start4 + i));
        }
    }

    return true;
}

/**
 * @brief Static configuration table for all supported Skydimo models
 * @details Each model can have multiple zones with different LED counts
 */
static const std::map<std::string, SkydimoModelConfig> skydimo_model_configs =
{
    // 2-zone models
    { "SK0201", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 20, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 20, 20 }
    }, 40 }},

    { "SK0202", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 30, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 30, 30 }
    }, 60 }},
    { "SK0F01", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 29, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 29, 29 }
    }, 58 }},

    { "SK0F02", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 25, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 25, 25 }
    }, 50 }},


    { "SK0204", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 25, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 25, 25 }
    }, 50 }},

    // 3-zone models
    { "SK0121", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 13, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 25, 13 },
        { "Zone 3", ZONE_TYPE_LINEAR, 13, 38 }
    }, 51 }},

    { "SK0124", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 14, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 26, 14 },
        { "Zone 3", ZONE_TYPE_LINEAR, 14, 40 }
    }, 54 }},

    { "SK0127", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 17, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 31, 17 },
        { "Zone 3", ZONE_TYPE_LINEAR, 17, 48 }
    }, 65 }},

    { "SK0132", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 20, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 37, 20 },
        { "Zone 3", ZONE_TYPE_LINEAR, 20, 57 }
    }, 77 }},

    { "SK0134", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 15, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 41, 15 },
        { "Zone 3", ZONE_TYPE_LINEAR, 15, 56 }
    }, 71 }},

    { "SK0149", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 19, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 69, 19 },
        { "Zone 3", ZONE_TYPE_LINEAR, 19, 88 }
    }, 107 }},

    // 4-zone models
    { "SK0L21", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 13, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 25, 13 },
        { "Zone 3", ZONE_TYPE_LINEAR, 13, 38 },
        { "Zone 4", ZONE_TYPE_LINEAR, 25, 51 }
    }, 76 }},

    { "SK0L24", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 14, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 26, 14 },
        { "Zone 3", ZONE_TYPE_LINEAR, 14, 40 },
        { "Zone 4", ZONE_TYPE_LINEAR, 26, 54 }
    }, 80 }},

    { "SK0L27", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 17, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 31, 17 },
        { "Zone 3", ZONE_TYPE_LINEAR, 17, 48 },
        { "Zone 4", ZONE_TYPE_LINEAR, 31, 65 }
    }, 96 }},

    { "SK0L32", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 20, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 37, 20 },
        { "Zone 3", ZONE_TYPE_LINEAR, 20, 57 },
        { "Zone 4", ZONE_TYPE_LINEAR, 37, 77 }
    }, 114 }},

    { "SK0L34", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 15, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 41, 15 },
        { "Zone 3", ZONE_TYPE_LINEAR, 15, 56 },
        { "Zone 4", ZONE_TYPE_LINEAR, 41, 71 }
    }, 112 }},

    // SKA series (3-zone models)
    { "SKA124", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 18, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 34, 18 },
        { "Zone 3", ZONE_TYPE_LINEAR, 18, 52 }
    }, 70 }},

    { "SKA127", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 20, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 41, 20 },
        { "Zone 3", ZONE_TYPE_LINEAR, 20, 61 }
    }, 81 }},

    { "SKA132", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 25, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 45, 25 },
        { "Zone 3", ZONE_TYPE_LINEAR, 25, 70 }
    }, 95 }},

    { "SKA134", { {
        { "Zone 1", ZONE_TYPE_LINEAR, 21, 0 },
        { "Zone 2", ZONE_TYPE_LINEAR, 51, 21 },
        { "Zone 3", ZONE_TYPE_LINEAR, 21, 72 }
    }, 93 }},

    // Single-zone LED strip models
    { "SK0402", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 72, 0 }
    }, 72 }},

    { "SK0403", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 96, 0 }
    }, 96 }},

    { "SK0404", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 144, 0 }
    }, 144 }},

    { "SK0901", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 14, 0 }
    }, 14 }},

    { "SK0801", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 2, 0 }
    }, 2 }},

    { "SK0802", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 18, 0 }
    }, 18 }},

    { "SK0803", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 10, 0 }
    }, 10 }},

    { "SK0E01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 16, 0 }
    }, 16 }},
    { "SK0H01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 2, 0 }
    }, 2 }},

    { "SK0H02", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 4, 0 }
    }, 4 }},

    { "SK0S01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 32, 0 }
    }, 32 }},

    { "SK0J01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 120, 0 }
    }, 120 }},

    { "SK0K01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 120, 0 }
    }, 120 }},

    { "SK0K02", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 15, 0 }
    }, 15 }},

    { "SK0M01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 24, 0 }
    }, 24 }},

    { "SK0N01", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 256, 0 }
    }, 256 }},

    { "SK0N02", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 1024, 0 }
    }, 1024 }},

    { "SK0N03", { {
        { "LED Strip", ZONE_TYPE_LINEAR, 253, 0 }
    }, 253 }}
};

/**
 * @brief Get model configuration by model ID
 * @param model_id The model identifier string
 * @return Pointer to the model configuration if found, nullptr otherwise
 */
inline const SkydimoModelConfig* GetSkydimoModelConfig(const std::string& model_id)
{
    auto it = skydimo_model_configs.find(model_id);
    if(it != skydimo_model_configs.end())
    {
        return &it->second;
    }
    return nullptr;
}

/**
 * @brief Extract model ID from device name
 * @param device_name Full device name (e.g., "Skydimo SK0121")
 * @return Model ID string (e.g., "SK0121") or empty string if not found
 */
inline std::string ExtractModelFromDeviceName(const std::string& device_name)
{
    // Look for "Skydimo " prefix
    const std::string prefix = "Skydimo ";
    size_t pos = device_name.find(prefix);
    if(pos != std::string::npos)
    {
        // Extract everything after "Skydimo "
        return device_name.substr(pos + prefix.length());
    }
    return "";
}
