/*
 * File: LedEditorDialog.h
 * Desc: LED editor modal dialog stub
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_WIDGETS_LEDEDITORDIALOG_H
#define LIGHTSYNC_WIDGETS_LEDEDITORDIALOG_H

#include "../../app/qt_compat.h"

class LedEditorDialog : public QWidget {
public:
    explicit LedEditorDialog(int device_id, QWidget* parent = nullptr);
    ~LedEditorDialog() override;
};

#endif // LIGHTSYNC_WIDGETS_LEDEDITORDIALOG_H


