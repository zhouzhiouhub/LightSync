/*
 * File: MainWindow.cpp
 * Desc: Primary window for LightSync UI
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "MainWindow.h"

#include "../app/qt_compat.h"

// Use logging macros only when OpenRGB headers are present
#if defined(__has_include) && __has_include("OpenRGB/LogManager.h")
#  include "OpenRGB/LogManager.h"
#  define LS_LOG_INFO(msg) LOG_INFO("%s", msg)
#else
#  define LS_LOG_INFO(msg) do {} while(0)
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* root = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* label = new QLabel("LightSync initialized", root);
    layout->addWidget(label);
    root->setLayout(layout);
    setCentralWidget(root);

    LS_LOG_INFO("MainWindow initialized");
}

MainWindow::~MainWindow() = default;


