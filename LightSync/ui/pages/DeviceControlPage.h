/*
 * File: DeviceControlPage.h
 * Desc: Device control page for non-edit devices
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_PAGES_DEVICECONTROLPAGE_H
#define LIGHTSYNC_PAGES_DEVICECONTROLPAGE_H

#include "../../app/qt_compat.h"

class DeviceControlPage : public QWidget {
public:
    explicit DeviceControlPage(int device_id, QWidget* parent = nullptr);
    ~DeviceControlPage() override;
};

#endif // LIGHTSYNC_PAGES_DEVICECONTROLPAGE_H


