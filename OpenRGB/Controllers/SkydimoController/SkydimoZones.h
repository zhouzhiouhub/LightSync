/*---------------------------------------------------------*\
| SkydimoZones.h                                            |
|                                                           |
|   Shared zone setup for Skydimo controllers (HID/Serial). |
|                                                           |
|   Skydimo Team                                08 Aug 2025 |
|                                                           |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkydimoDeviceConfig.h"
#include <string>
#include <vector>

namespace SkydimoZones
{
    inline void setupZonesForDeviceName(const std::string& device_name,
                                        std::vector<zone>& zones_out,
                                        std::vector<led>& leds_out)
    {
        zones_out.clear();
        leds_out.clear();

        std::string model_id = ExtractModelFromDeviceName(device_name);
        const SkydimoModelConfig* config = GetSkydimoModelConfig(model_id);

        if(config != nullptr)
        {
            unsigned int m_w = 0, m_h = 0;
            std::vector<unsigned int> m;
            if(config->zones.size() >= 2 && config->zones.size() <= 4 && BuildSkydimoMatrixForModel(*config, m_w, m_h, m))
            {
                zone matrix_zone;
                matrix_zone.name         = "Matrix";
                matrix_zone.type         = ZONE_TYPE_MATRIX;
                matrix_zone.leds_min     = config->total_leds;
                matrix_zone.leds_max     = config->total_leds;
                matrix_zone.leds_count   = config->total_leds;
                matrix_zone.matrix_map   = new matrix_map_type;
                matrix_zone.matrix_map->height = m_h;
                matrix_zone.matrix_map->width  = m_w;
                unsigned int map_size = m_w * m_h;
                unsigned int *map_buf = new unsigned int[map_size];
                for(unsigned int i = 0; i < map_size; ++i) map_buf[i] = m[i];
                matrix_zone.matrix_map->map = map_buf;

                zones_out.push_back(matrix_zone);

                for(int i = 0; i < config->total_leds; i++)
                {
                    led new_led;
                    new_led.name = std::string("LED ") + std::to_string(i + 1);
                    leds_out.push_back(new_led);
                }
            }
            else
            {
                for(size_t zone_idx = 0; zone_idx < config->zones.size(); zone_idx++)
                {
                    const SkydimoZoneConfig& zone_config = config->zones[zone_idx];
                    zone new_zone;
                    new_zone.name         = zone_config.name;
                    new_zone.type         = zone_config.type;
                    new_zone.leds_min     = zone_config.led_count;
                    new_zone.leds_max     = zone_config.led_count;
                    new_zone.leds_count   = zone_config.led_count;
                    new_zone.matrix_map   = nullptr;
                    zones_out.push_back(new_zone);

                    for(int i = 0; i < zone_config.led_count; i++)
                    {
                        led new_led;
                        new_led.name = zone_config.name + " LED " + std::to_string(i + 1);
                        leds_out.push_back(new_led);
                    }
                }
            }
        }
        else
        {
            // fallback: single zone strip
            zone strip_zone;
            strip_zone.name         = "LED Strip";
            strip_zone.type         = ZONE_TYPE_LINEAR;
            strip_zone.leds_min     = 1;
            strip_zone.leds_max     = 2000;
            strip_zone.leds_count   = 3;
            strip_zone.matrix_map   = nullptr;
            zones_out.push_back(strip_zone);

            for(int i = 0; i < strip_zone.leds_count; i++)
            {
                led new_led;
                new_led.name = "LED " + std::to_string(i + 1);
                leds_out.push_back(new_led);
            }
        }
    }
}


