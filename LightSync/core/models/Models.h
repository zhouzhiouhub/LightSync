/*
 * File: Models.h
 * Desc: Core data models used across services and UI
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_MODELS_H
#define LIGHTSYNC_MODELS_H

#include <vector>
#include <string>
#include <cstdint>

enum class EffectMode { Builtin, ScreenSync, Music };

struct LedNode {
    int device_id;
    int led_index;
    int x;
    int y;
    int w;
    int h;
};

struct DeviceConfig {
    int device_id;
    int brightness;  // 0..100
    bool enabled;
    EffectMode mode;
};

struct ProfileConfig {
    int fps;
    int quality;
    bool gamma_correct;
    std::vector<DeviceConfig> devices;
};

#endif // LIGHTSYNC_MODELS_H


