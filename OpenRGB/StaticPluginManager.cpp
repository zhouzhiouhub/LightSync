/*---------------------------------------------------------*\
| StaticPluginManager.cpp                                   |
|                                                           |
|   OpenRGB static plugin manager                           |
|   Manages plugins compiled directly into the application  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "StaticPluginManager.h"
#include "LogManager.h"
#include "ResourceManager.h"

// Include the static plugins - use relative paths from OpenRGB directory
#include "../OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.h"
#include "../OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.h"

StaticPluginManager::StaticPluginManager()
{
    AddPluginCallbackVal    = nullptr;
    AddPluginCallbackArg    = nullptr;
    
    LOG_INFO("[StaticPluginManager] Initialized");
}

StaticPluginManager::~StaticPluginManager()
{
    UnloadStaticPlugins();
}

void StaticPluginManager::RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg)
{
    AddPluginCallbackVal    = new_callback;
    AddPluginCallbackArg    = new_callback_arg;
}

void StaticPluginManager::LoadStaticPlugins()
{
    LOG_INFO("[StaticPluginManager] Loading static plugins");

    /*---------------------------------------------------------*\
    | Create and register OpenRGBEffectsPlugin                  |
    | This plugin is built-in and always available              |
    \*---------------------------------------------------------*/
    OpenRGBEffectsPlugin* effects_plugin = new OpenRGBEffectsPlugin();
    RegisterPlugin(effects_plugin);

    /*---------------------------------------------------------*\
    | Create and register OpenRGBVisualMapPlugin                |
    | This plugin is built-in and always available              |
    \*---------------------------------------------------------*/
    OpenRGBVisualMapPlugin* visual_map_plugin = new OpenRGBVisualMapPlugin();
    RegisterPlugin(visual_map_plugin);

    LOG_INFO("[StaticPluginManager] Loaded %zu static plugins", ActivePlugins.size());
    LOG_INFO("[StaticPluginManager] Note: Additional plugins can still be loaded from user config directory");
}

void StaticPluginManager::RegisterPlugin(OpenRGBPluginInterface* plugin)
{
    if (!plugin)
    {
        LOG_ERROR("[StaticPluginManager] Attempted to register null plugin");
        return;
    }

    /*---------------------------------------------------------*\
    | Verify API version compatibility                          |
    \*---------------------------------------------------------*/
    if (plugin->GetPluginAPIVersion() != OPENRGB_PLUGIN_API_VERSION)
    {
        LOG_WARNING("[StaticPluginManager] Plugin API version mismatch");
        delete plugin;
        return;
    }

    /*---------------------------------------------------------*\
    | Get plugin info                                           |
    \*---------------------------------------------------------*/
    OpenRGBPluginInfo info = plugin->GetPluginInfo();
    
    LOG_INFO("[StaticPluginManager] Registering plugin: %s", info.Name.c_str());

    /*---------------------------------------------------------*\
    | Create plugin entry                                       |
    \*---------------------------------------------------------*/
    OpenRGBPluginEntry entry;
    entry.info          = info;
    entry.plugin        = plugin;
    entry.loader        = nullptr;  // No QPluginLoader for static plugins
    entry.path          = "";       // No file path for static plugins
    entry.enabled       = true;     // Static plugins are always enabled
    entry.widget        = nullptr;
    entry.incompatible  = false;
    entry.api_version   = plugin->GetPluginAPIVersion();
    entry.is_system     = true;     // Treat as system plugin

    ActivePlugins.push_back(entry);
    
    /*---------------------------------------------------------*\
    | Load the plugin immediately                               |
    \*---------------------------------------------------------*/
    LoadPlugin(&ActivePlugins.back());
}

void StaticPluginManager::LoadPlugin(OpenRGBPluginEntry* plugin_entry)
{
    if (!plugin_entry || !plugin_entry->plugin)
    {
        return;
    }

    LOG_INFO("[StaticPluginManager] Loading plugin: %s", plugin_entry->info.Name.c_str());

    /*---------------------------------------------------------*\
    | Initialize the plugin with ResourceManager                |
    \*---------------------------------------------------------*/
    plugin_entry->plugin->Load(ResourceManager::get());

    /*---------------------------------------------------------*\
    | Call the Add Plugin callback                             |
    \*---------------------------------------------------------*/
    if (AddPluginCallbackArg != nullptr && AddPluginCallbackVal != nullptr)
    {
        AddPluginCallbackVal(AddPluginCallbackArg, plugin_entry);
    }
}

void StaticPluginManager::UnloadPlugin(OpenRGBPluginEntry* plugin_entry)
{
    if (!plugin_entry || !plugin_entry->plugin)
    {
        return;
    }

    LOG_INFO("[StaticPluginManager] Unloading plugin: %s", plugin_entry->info.Name.c_str());

    /*---------------------------------------------------------*\
    | Call plugin's Unload method                               |
    \*---------------------------------------------------------*/
    plugin_entry->plugin->Unload();
}

void StaticPluginManager::UnloadStaticPlugins()
{
    LOG_INFO("[StaticPluginManager] Unloading all static plugins");

    for (auto& entry : ActivePlugins)
    {
        UnloadPlugin(&entry);
        
        // Delete the plugin instance
        if (entry.plugin)
        {
            delete entry.plugin;
            entry.plugin = nullptr;
        }
    }

    ActivePlugins.clear();
}

