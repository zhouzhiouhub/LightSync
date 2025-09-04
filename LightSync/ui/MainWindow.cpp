/*
 * File: MainWindow.cpp
 * Desc: Primary window for LightSync UI
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "MainWindow.h"

#include "../app/qt_compat.h"
#include "pages/HomePage.h"
// OpenRGB core headers (use project-relative paths per .pro INCLUDEPATH)
#include "../../OpenRGB/ResourceManager.h"
#include "../../OpenRGB/RGBController/RGBController.h"

// Use logging macros only when OpenRGB headers are present
#include "../../OpenRGB/LogManager.h"
#define LS_LOG_INFO(msg) LOG_INFO("%s", msg)

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create HomePage and wire navigation
    HomePage* home = new HomePage(this);
    setCentralWidget(home);

    // Initialize OpenRGB core; prefer connecting to a running OpenRGB SDK server
    // Enable console logging for visibility in Creator's Application Output
    LogManager::get()->setVerbosity(LL_INFO);
    LogManager::get()->log_console_enabled = true;

    // Try connecting to OpenRGB SDK server (preferred), also allow local detection
    // tryConnect=true, detectDevices=true, startServer=false, applyPostOptions=false
    ResourceManager::get()->Initialize(true, true, false, false);
    ResourceManager::get()->RegisterDeviceListChangeCallback(
        [](void* arg){ static_cast<MainWindow*>(arg)->OnDeviceListChanged(); }, this);
    // Ensure an initial refresh if detection already finished very quickly
    OnDeviceListChanged();

    // Initial population
    RefreshDevices();

    // Default window size
    setMinimumSize(800, 600);
    resize(800, 600);

    LS_LOG_INFO("HomePage shown");
}

MainWindow::~MainWindow() = default;


void MainWindow::OnDeviceListChanged()
{
    // Ensure UI updates happen on the GUI thread
    QMetaObject::invokeMethod(this, [this]() { RefreshDevices(); }, Qt::QueuedConnection);
}

static std::string device_type_to_str_local(device_type t)
{
    switch(t)
    {
        case DEVICE_TYPE_KEYBOARD: return "keyboard";
        case DEVICE_TYPE_MOUSE: return "mouse";
        case DEVICE_TYPE_HEADSET: return "headset";
        case DEVICE_TYPE_LEDSTRIP: return "strip";
        case DEVICE_TYPE_MOTHERBOARD: return "mainboard";
        case DEVICE_TYPE_GPU: return "gpu";
        default: return "device";
    }
}

void MainWindow::RefreshDevices()
{
    // Cast central widget to HomePage; in Qt builds, qobject_cast would be used
    HomePage* home = static_cast<HomePage*>(centralWidget());
    if(!home) return;

    std::vector<DeviceSummary> devices;
    std::vector<RGBController*>& ctrls = ResourceManager::get()->GetRGBControllers();
    for(size_t i = 0; i < ctrls.size(); ++i)
    {
        RGBController* c = ctrls[i];
        DeviceSummary d;
        d.device_id = static_cast<int>(i);
        d.name = c->GetName();
        d.need_led_edit = true;
        d.type = device_type_to_str_local(c->type);
        devices.push_back(d);
    }
    home->SetDevices(devices);
}


