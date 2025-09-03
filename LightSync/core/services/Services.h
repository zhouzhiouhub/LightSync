/*
 * File: Services.h
 * Desc: Core services stubs for LightSync
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_SERVICES_H
#define LIGHTSYNC_SERVICES_H

#include <vector>
#include <cstdint>

class DeviceService {
public:
    void RefreshDevices() {}
    void SetBrightness(int device_id, int brightness) { (void)device_id; (void)brightness; }
    void SendFrame(const std::vector<uint32_t>& rgb) { (void)rgb; }
};

class EffectService {
public:
    void Start() {}
    void Stop() {}
};

class LayoutService {
public:
    void Reset() {}
};

class ScreenCaptureService {
public:
    void Configure(int fps, int quality) { (void)fps; (void)quality; }
};

class AudioCaptureService {
public:
    void Configure(int sample_rate) { (void)sample_rate; }
};

class MappingService {
public:
    void Reset() {}
};

class ProfileService {
public:
    void LoadDefault() {}
};

class NetworkService {
public:
    void EnableServer(int port) { (void)port; }
};

#endif // LIGHTSYNC_SERVICES_H


