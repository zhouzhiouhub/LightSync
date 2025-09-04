/*
 * File: MainWindow.cpp
 * Desc: Primary window for LightSync UI
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#include "MainWindow.h"

#include "../app/qt_compat.h"
#include "pages/HomePage.h"
// Avoid requiring QtCore debug in linters

// OpenRGB core headers (guard for builds without full OpenRGB include path)
#if defined(__has_include) && __has_include("SPDWrapper.h") && __has_include("../../OpenRGB/ResourceManager.h")
#  include "../../OpenRGB/ResourceManager.h"
#  include "../../OpenRGB/RGBController/RGBController.h"
#  define HAVE_OPENRGB_CORE 1
#else
#  define HAVE_OPENRGB_CORE 0
   enum device_type { DEVICE_TYPE_UNKNOWN = 0, DEVICE_TYPE_KEYBOARD, DEVICE_TYPE_MOUSE, DEVICE_TYPE_HEADSET, DEVICE_TYPE_LEDSTRIP, DEVICE_TYPE_MOTHERBOARD, DEVICE_TYPE_GPU };
   struct RGBController { device_type type { DEVICE_TYPE_UNKNOWN }; std::string GetName() const { return "Device"; } };
   class ResourceManager {
   public:
       static ResourceManager* get() { static ResourceManager inst; return &inst; }
       void Initialize(bool, bool, bool, bool) {}
       void RegisterDeviceListChangeCallback(void (*)(void*), void*) {}
       std::vector<RGBController*>& GetRGBControllers() { static std::vector<RGBController*> empty; return empty; }
   };
#endif

// Use logging macros only when OpenRGB headers are present
#if defined(__has_include) && __has_include("OpenRGB/LogManager.h")
# include "OpenRGB/LogManager.h"
# define LS_LOG_INFO(msg) LOG_INFO("%s", msg)
#else
#  define LS_LOG_INFO(msg) do {} while(0)
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create HomePage and wire navigation
    HomePage* home = new HomePage(this);
    setCentralWidget(home);

    // Initialize OpenRGB core in-process and start device detection when available
#if HAVE_OPENRGB_CORE
    ResourceManager::get()->Initialize(false, true, false, false);
    ResourceManager::get()->RegisterDeviceListChangeCallback(
        [](void* arg){ static_cast<MainWindow*>(arg)->OnDeviceListChanged(); }, this);
#endif

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
    RefreshDevices();
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


