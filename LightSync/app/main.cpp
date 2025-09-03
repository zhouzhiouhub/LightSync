/*
 * File: main.cpp
 * Desc: Entry point for LightSync application
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "qt_compat.h"
// JSON will be added when configuration is implemented

#include "../ui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Logging and profile initialization will be configured later per docs

    MainWindow w;
    w.show();
    return app.exec();
}


