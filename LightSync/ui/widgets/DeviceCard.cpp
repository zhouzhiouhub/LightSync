/*
 * File: DeviceCard.cpp
 * Desc: Clickable device card widget
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "DeviceCard.h"

#include "../../app/qt_compat.h"

class QMouseEvent;

DeviceCard::DeviceCard(const DeviceSummary& summary, QWidget* parent)
    : QWidget(parent), summary_(summary)
{
    QVBoxLayout* v = new QVBoxLayout();

    // Colored block for type differentiation
    QWidget* color_block = new QWidget(this);
    color_block->setFixedSize(99, 70);

    const char* color = "#D0D0D0";
    if(summary.type == "keyboard") color = "#FFE4B5";      // moccasin
    else if(summary.type == "mouse") color = "#E0FFFF";     // light cyan
    else if(summary.type == "headset") color = "#E6E6FA";   // lavender
    else if(summary.type == "strip") color = "#F0FFF0";     // honeydew

    // Card border
    setStyleSheet("border:1px solid #cccccc; border-radius:6px;");
    color_block->setStyleSheet((std::string("background:") + color + "; border-top-left-radius:6px; border-top-right-radius:6px;").c_str());

    QLabel* name = new QLabel(summary.name.c_str(), this);
    name->setAlignment(Qt::AlignCenter);

    // put name below the colored block
    v->addWidget(color_block);
    v->addWidget(name);
    v->addStretch(1);

    setFixedSize(99, 99);
    setLayout(v);
}

DeviceCard::~DeviceCard() = default;

void DeviceCard::mousePressEvent(QMouseEvent* /*event*/)
{
    if(on_clicked_) on_clicked_(summary_.device_id, summary_.need_led_edit);
}


