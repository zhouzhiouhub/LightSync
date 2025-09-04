/*
 * File: DeviceControlPage.cpp
 * Desc: Device control page for non-edit devices
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "DeviceControlPage.h"

#include "../../app/qt_compat.h"

DeviceControlPage::DeviceControlPage(int device_id, QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout();
    QLabel* title = new QLabel("Device Control", this);
    v->addWidget(title);
    QLabel* id = new QLabel((std::string("ID: ") + std::to_string(device_id)).c_str(), this);
    v->addWidget(id);
    setLayout(v);
}

DeviceControlPage::~DeviceControlPage() = default;


