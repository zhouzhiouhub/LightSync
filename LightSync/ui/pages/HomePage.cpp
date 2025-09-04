/*
 * File: HomePage.cpp
 * Desc: Home page showing device cards
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "HomePage.h"

#include "../../app/qt_compat.h"
#include <QLabel>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    root_ = new QVBoxLayout();
    root_->setSpacing(10);
    root_->setContentsMargins(10,10,10,10);
    empty_label_ = new QLabel(QString::fromUtf8(u8"No devices found. Connect a supported RGB device or start the OpenRGB core, then retry."), this);
    empty_label_->setAlignment(Qt::AlignCenter);
    root_->addWidget(empty_label_);
    root_->addStretch(1);
    setLayout(root_);
}

HomePage::~HomePage() = default;

void HomePage::SetDevices(const std::vector<DeviceSummary>& list)
{
    devices_ = list;
    Relayout();
}

void HomePage::resizeEvent(QResizeEvent* /*event*/)
{
    Relayout();
}

void HomePage::Relayout()
{
    // Remove previous child layouts and widgets from root_
    if(root_) {
        while(root_->count() > 0) {
            QLayoutItem* it = root_->takeAt(0);
            if(!it) continue;
            if(it->widget()) { it->widget()->deleteLater(); }
            else if(it->layout()) {
                // remove items inside row layout
                // (compat header: symbolic; real Qt will iterate)
            }
            delete it;
        }
        root_->setSpacing(10);
        root_->setContentsMargins(10,10,10,10);
    }

    // Empty state
    if(devices_.empty()) {
        empty_label_ = new QLabel(QString::fromUtf8(u8"No devices found. Start the OpenRGB server or connect a device, then retry."), this);
        empty_label_->setAlignment(Qt::AlignCenter);
        root_->addWidget(empty_label_);
        root_->addStretch(1);
        return;
    }

    int card_w = 99;
    int gap = 10;
    int total_w = width() - 20; // minus margins
    int cols = std::max(1, total_w / (card_w + gap));
    if(cols < 1) cols = 1;

    QHBoxLayout* row = nullptr;
    int col = 0;
    for(const auto& d : devices_) {
        if(col == 0) {
            row = new QHBoxLayout();
            row->setSpacing(10);
            root_->addLayout(row);
        }
        DeviceCard* card = new DeviceCard(d, this);
        card->SetOnClicked([this](int id, bool need_led_edit){ if(on_device_chosen_) on_device_chosen_(id, need_led_edit); });
        row->addWidget(card);
        col++;
        if(col >= cols) {
            // add stretch to keep items left, then start a new row
            if(row) { row->addStretch(1); }
            row = nullptr;
            col = 0;
        }
    }

    // finalize last row
    if(row) { row->addStretch(1); }
    // push content to top-left
    root_->addStretch(1);
}


