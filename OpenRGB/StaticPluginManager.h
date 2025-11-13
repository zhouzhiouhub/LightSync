#ifndef STATIC_PLUGIN_MANAGER_H
#define STATIC_PLUGIN_MANAGER_H

#include "OpenRGBPluginInterface.h"
#include "PluginManager.h"
#include <vector>

/*---------------------------------------------------------*\
| StaticPluginManager                                       |
|                                                           |
|   Manages statically compiled/linked plugins              |
|   Plugins are instantiated directly rather than loaded    |
|   dynamically from DLL/SO files                           |
|                                                           |
|   Note: This works alongside PluginManager, which still   |
|   supports loading additional plugins from the user's     |
|   configuration directory (e.g., ~/.config/OpenRGB/plugins|
|   or %APPDATA%/OpenRGB/plugins)                           |
\*---------------------------------------------------------*/

class StaticPluginManager
{
public:
    StaticPluginManager();
    ~StaticPluginManager();

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg);
    void LoadStaticPlugins();
    void UnloadStaticPlugins();

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    void RegisterPlugin(OpenRGBPluginInterface* plugin);
    void LoadPlugin(OpenRGBPluginEntry* plugin_entry);
    void UnloadPlugin(OpenRGBPluginEntry* plugin_entry);

    AddPluginCallback       AddPluginCallbackVal;
    void *                  AddPluginCallbackArg;
};

#endif // STATIC_PLUGIN_MANAGER_H

