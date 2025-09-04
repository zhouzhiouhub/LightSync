/*
 * File: HomePage.h
 * Desc: Home page showing device cards
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_PAGES_HOMEPAGE_H
#define LIGHTSYNC_PAGES_HOMEPAGE_H

#include "../../app/qt_compat.h"
#include "../widgets/DeviceCard.h"
#include <vector>
#include <functional>

class HomePage : public QWidget {
public:
    explicit HomePage(QWidget* parent = nullptr);
    ~HomePage() override;

    void SetDevices(const std::vector<DeviceSummary>& list);
    void SetOnDeviceChosen(std::function<void(int, bool)> cb) { on_device_chosen_ = cb; }

private:
    QVBoxLayout* root_ {nullptr};
    QHBoxLayout* current_row_ {nullptr};
    std::function<void(int, bool)> on_device_chosen_;
    std::vector<DeviceSummary> devices_;
protected:
    void resizeEvent(QResizeEvent* event) override;
    void Relayout();
};

#endif // LIGHTSYNC_PAGES_HOMEPAGE_H


