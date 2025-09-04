/*
 * File: DeviceCard.h
 * Desc: Clickable device card widget
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_WIDGETS_DEVICECARD_H
#define LIGHTSYNC_WIDGETS_DEVICECARD_H

#include "../../app/qt_compat.h"
#include <functional>

struct DeviceSummary {
    int device_id;
    std::string name;
    bool need_led_edit;
    std::string type; // e.g., "keyboard", "mouse", "headset", ...
};

class DeviceCard : public QWidget {
public:
    explicit DeviceCard(const DeviceSummary& summary, QWidget* parent = nullptr);
    ~DeviceCard() override;

    void SetOnClicked(std::function<void(int, bool)> callback) { on_clicked_ = callback; }

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    DeviceSummary summary_;
    std::function<void(int, bool)> on_clicked_;
};

#endif // LIGHTSYNC_WIDGETS_DEVICECARD_H


