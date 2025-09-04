/*
 * File: LedEditorDialog.cpp
 * Desc: LED editor modal dialog stub
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "LedEditorDialog.h"

#include "../../app/qt_compat.h"

LedEditorDialog::LedEditorDialog(int device_id, QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout();
    QLabel* title = new QLabel("灯珠编辑", this);
    v->addWidget(title);
    QLabel* id = new QLabel((std::string("设备 ID: ") + std::to_string(device_id)).c_str(), this);
    v->addWidget(id);
    setLayout(v);
}

LedEditorDialog::~LedEditorDialog() = default;


