/*
 * File: MainWindow.h
 * Desc: Primary window for LightSync UI
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_MAINWINDOW_H
#define LIGHTSYNC_MAINWINDOW_H

#include "../app/qt_compat.h"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};

#endif // LIGHTSYNC_MAINWINDOW_H


