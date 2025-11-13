#-----------------------------------------------------------------------------------------------#
# Static Plugins Configuration                                                                   #
#   This file includes all source files from plugins to be statically linked                     #
#-----------------------------------------------------------------------------------------------#

DEFINES += STATIC_PLUGINS_ENABLED

#-----------------------------------------------------------------------------------------------#
# Static Plugin Manager                                                                         #
#-----------------------------------------------------------------------------------------------#
HEADERS +=                                                                                      \
    StaticPluginManager.h                                                                       \

SOURCES +=                                                                                      \
    StaticPluginManager.cpp                                                                     \

#-----------------------------------------------------------------------------------------------#
# Visual Map Plugin Version Info                                                                #
#-----------------------------------------------------------------------------------------------#
win32:DEFINES +=                                                                               \
    VISUALMAP_LATEST_BUILD_URL=\\"\"\"https://gitlab.com/OpenRGBDevelopers/openrgbvisualmapplugin/-/jobs/artifacts/master/download?job=Windows 64\\"\"\" \
unix:!macx:DEFINES +=                                                                           \
    VISUALMAP_LATEST_BUILD_URL=\\"\"\"https://gitlab.com/OpenRGBDevelopers/openrgbvisualmapplugin/-/jobs/artifacts/master/download?job=Linux 64\\"\"\" \

#-----------------------------------------------------------------------------------------------#
# OpenRGB Visual Map Plugin - Statically Linked                                                 #
# NOTE: MUST be included BEFORE OpenRGBEffectsPlugin to resolve ControllerZone.h conflict      #
#       Both plugins have a ControllerZone.h, and VisualMapPlugin needs its own version         #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    ../OpenRGBVisualMapPlugin                                                                   \
    ../OpenRGBVisualMapPlugin/ui                                                                \
    ../OpenRGBVisualMapPlugin/ui/widget-editor                                                  \

HEADERS +=                                                                                      \
    ../OpenRGBVisualMapPlugin/OpenRGBPluginsFont.h                                              \
    ../OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.h                                          \
    ../OpenRGBVisualMapPlugin/ZoneManager.h                                                     \
    ../OpenRGBVisualMapPlugin/VirtualController.h                                               \
    ../OpenRGBVisualMapPlugin/VisualMapSettingsManager.h                                        \
    ../OpenRGBVisualMapPlugin/VisualMapJsonDefinitions.h                                        \
    ../OpenRGBVisualMapPlugin/VisualMapControllerZone.h                                        \
    ../OpenRGBVisualMapPlugin/ui/ClickableLabel.h                                               \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.h                                                  \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.h                                                    \
    ../OpenRGBVisualMapPlugin/ui/ControllerZoneItem.h                                           \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.h                                                   \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/EditableLabel.h                                                \
    ../OpenRGBVisualMapPlugin/ui/GradientPresets.h                                              \
    ../OpenRGBVisualMapPlugin/ui/GridSettings.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.h                                                   \
    ../OpenRGBVisualMapPlugin/ui/Scene.h                                                        \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.h                                                    \
    ../OpenRGBVisualMapPlugin/ui/TooltipProxy.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.h                                         \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/EditorGrid.h                                     \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/LedItem.h                                        \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.h                                            \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.h                                          \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.h                                   \
    ../OpenRGBVisualMapPlugin/ui/Grid.h                                                         \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.h                                                  \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.h                                                  \

SOURCES +=                                                                                      \
    ../OpenRGBVisualMapPlugin/OpenRGBPluginsFont.cpp                                            \
    ../OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.cpp                                        \
    ../OpenRGBVisualMapPlugin/VirtualController.cpp                                             \
    ../OpenRGBVisualMapPlugin/VisualMapSettingsManager.cpp                                      \
    ../OpenRGBVisualMapPlugin/ZoneManager.cpp                                                   \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.cpp                                          \
    ../OpenRGBVisualMapPlugin/ui/ClickableLabel.cpp                                             \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.cpp                                                  \
    ../OpenRGBVisualMapPlugin/ui/ControllerZoneItem.cpp                                         \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.cpp                                                 \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.cpp                                               \
    ../OpenRGBVisualMapPlugin/ui/EditableLabel.cpp                                              \
    ../OpenRGBVisualMapPlugin/ui/Grid.cpp                                                       \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.cpp                                        \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.cpp                                                 \
    ../OpenRGBVisualMapPlugin/ui/Scene.cpp                                                      \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.cpp                                                  \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.cpp                                       \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/EditorGrid.cpp                                   \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/LedItem.cpp                                      \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.cpp                                 \

FORMS +=                                                                                        \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.ui                                           \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.ui                                                   \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.ui                                                  \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.ui                                                \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.ui                                         \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.ui                                                  \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.ui                                                   \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.ui                                        \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.ui                                  \

#-----------------------------------------------------------------------------------------------#
# OpenRGB Effects Plugin - Statically Linked                                                    #
#-----------------------------------------------------------------------------------------------#
win32:DEFINES +=                                                                               \
    EFFECTS_LATEST_BUILD_URL=\\"\"\"https://gitlab.com/OpenRGBDevelopers/OpenRGBEffectsPlugin/-/jobs/artifacts/master/download?job=Windows 64\\"\"\" \
unix:!macx:DEFINES +=                                                                           \
    EFFECTS_LATEST_BUILD_URL=\\"\"\"https://gitlab.com/OpenRGBDevelopers/OpenRGBEffectsPlugin/-/jobs/artifacts/master/download?job=Linux 64\\"\"\" \

INCLUDEPATH +=                                                                                  \
    ../OpenRGBEffectsPlugin                                                                     \
    ../OpenRGBEffectsPlugin/Audio                                                               \
    ../OpenRGBEffectsPlugin/Effects                                                             \
    ../OpenRGBEffectsPlugin/ScreenCapturer                                                      \
    ../OpenRGBEffectsPlugin/ScreenCapturer/windows                                              \
    ../OpenRGBEffectsPlugin/ScreenCapturer/qt                                                   \
    ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland                                        \
    ../OpenRGBEffectsPlugin/Dependencies/ctkrangeslider                                         \
    ../OpenRGBEffectsPlugin/Dependencies/chuck_fft                                              \

HEADERS +=                                                                                      \
    ../OpenRGBEffectsPlugin/Audio/AudioDataStruct.h                                             \
    ../OpenRGBEffectsPlugin/Audio/AudioSettings.h                                               \
    ../OpenRGBEffectsPlugin/Audio/AudioSettingsStruct.h                                         \
    ../OpenRGBEffectsPlugin/Audio/AudioSignalProcessor.h                                        \
    ../OpenRGBEffectsPlugin/EffectsColorPicker.h                                                \
    ../OpenRGBEffectsPlugin/ColorsPicker.h                                                      \
    ../OpenRGBEffectsPlugin/ColorUtils.h                                                        \
    ../OpenRGBEffectsPlugin/ControllerZone.h                                                    \
    ../OpenRGBEffectsPlugin/EffectsDeviceList.h                                                 \
    ../OpenRGBEffectsPlugin/DeviceListItem.h                                                    \
    ../OpenRGBEffectsPlugin/EffectList.h                                                        \
    ../OpenRGBEffectsPlugin/EffectListManager.h                                                 \
    ../OpenRGBEffectsPlugin/EffectManager.h                                                     \
    ../OpenRGBEffectsPlugin/EffectsName.h                                                       \
    ../OpenRGBEffectsPlugin/EffectSearch.h                                                      \
    ../OpenRGBEffectsPlugin/EffectTabHeader.h                                                   \
    ../OpenRGBEffectsPlugin/Effects/EffectRegisterer.h                                          \
    ../OpenRGBEffectsPlugin/Effects/RGBEffect.h                                                 \
    ../OpenRGBEffectsPlugin/LivePreviewController.h                                             \
    ../OpenRGBEffectsPlugin/OpenRGBEffectPage.h                                                 \
    ../OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.h                                              \
    ../OpenRGBEffectsPlugin/OpenRGBEffectSettings.h                                             \
    ../OpenRGBEffectsPlugin/OpenRGBEffectTab.h                                                  \
    ../OpenRGBEffectsPlugin/PluginInfo.h                                                        \
    ../OpenRGBEffectsPlugin/PreviewWidget.h                                                     \
    ../OpenRGBEffectsPlugin/QTooltipedSlider.h                                                  \
    ../OpenRGBEffectsPlugin/SaveProfilePopup.h                                                  \
    ../OpenRGBEffectsPlugin/ZoneListItem.h                                                      \
    ../OpenRGBEffectsPlugin/OpenRGBPluginsFont.h                                                \
    ../OpenRGBEffectsPlugin/GlobalSettings.h                                                    \
    ../OpenRGBEffectsPlugin/ScreenCapturer/ScreenCapturer.h                                     \
    ../OpenRGBEffectsPlugin/ScreenCapturer/qt/QtScreenCapturer.h                                \
    ../OpenRGBEffectsPlugin/Dependencies/ctkrangeslider/ctkrangeslider.h                        \
    ../OpenRGBEffectsPlugin/Dependencies/chuck_fft/chuck_fft.h                                  \

SOURCES +=                                                                                      \
    ../OpenRGBEffectsPlugin/Audio/AudioSettings.cpp                                             \
    ../OpenRGBEffectsPlugin/Audio/AudioSettingsStruct.cpp                                       \
    ../OpenRGBEffectsPlugin/Audio/AudioSignalProcessor.cpp                                      \
    ../OpenRGBEffectsPlugin/Audio/AudioManager.cpp                                              \
    ../OpenRGBEffectsPlugin/EffectsColorPicker.cpp                                              \
    ../OpenRGBEffectsPlugin/ColorsPicker.cpp                                                    \
    ../OpenRGBEffectsPlugin/EffectsDeviceList.cpp                                               \
    ../OpenRGBEffectsPlugin/DeviceListItem.cpp                                                  \
    ../OpenRGBEffectsPlugin/EffectList.cpp                                                      \
    ../OpenRGBEffectsPlugin/EffectListManager.cpp                                               \
    ../OpenRGBEffectsPlugin/EffectManager.cpp                                                   \
    ../OpenRGBEffectsPlugin/EffectSearch.cpp                                                    \
    ../OpenRGBEffectsPlugin/EffectTabHeader.cpp                                                 \
    ../OpenRGBEffectsPlugin/LivePreviewController.cpp                                           \
    ../OpenRGBEffectsPlugin/OpenRGBEffectPage.cpp                                               \
    ../OpenRGBEffectsPlugin/OpenRGBEffectsPlugin.cpp                                            \
    ../OpenRGBEffectsPlugin/OpenRGBEffectSettings.cpp                                           \
    ../OpenRGBEffectsPlugin/OpenRGBEffectTab.cpp                                                \
    ../OpenRGBEffectsPlugin/PluginInfo.cpp                                                      \
    ../OpenRGBEffectsPlugin/QTooltipedSlider.cpp                                                \
    ../OpenRGBEffectsPlugin/PreviewWidget.cpp                                                   \
    ../OpenRGBEffectsPlugin/SaveProfilePopup.cpp                                                \
    ../OpenRGBEffectsPlugin/ZoneListItem.cpp                                                    \
    ../OpenRGBEffectsPlugin/OpenRGBPluginsFont.cpp                                              \
    ../OpenRGBEffectsPlugin/GlobalSettings.cpp                                                  \
    ../OpenRGBEffectsPlugin/ScreenCapturer/qt/QtScreenCapturer.cpp                              \
    ../OpenRGBEffectsPlugin/Dependencies/ctkrangeslider/ctkrangeslider.cpp                      \
    ../OpenRGBEffectsPlugin/Dependencies/chuck_fft/chuck_fft.c                                  \

# Add all effect implementations
include(../OpenRGBEffectsPlugin/effects_list.pri)

FORMS +=                                                                                        \
    ../OpenRGBEffectsPlugin/Audio/AudioSettings.ui                                              \
    ../OpenRGBEffectsPlugin/EffectsColorPicker.ui                                               \
    ../OpenRGBEffectsPlugin/ColorsPicker.ui                                                     \
    ../OpenRGBEffectsPlugin/EffectsDeviceList.ui                                                \
    ../OpenRGBEffectsPlugin/DeviceListItem.ui                                                   \
    ../OpenRGBEffectsPlugin/EffectList.ui                                                       \
    ../OpenRGBEffectsPlugin/EffectSearch.ui                                                     \
    ../OpenRGBEffectsPlugin/EffectTabHeader.ui                                                  \
    ../OpenRGBEffectsPlugin/LivePreviewController.ui                                            \
    ../OpenRGBEffectsPlugin/OpenRGBEffectPage.ui                                                \
    ../OpenRGBEffectsPlugin/OpenRGBEffectTab.ui                                                 \
    ../OpenRGBEffectsPlugin/PluginInfo.ui                                                       \
    ../OpenRGBEffectsPlugin/SaveProfilePopup.ui                                                 \
    ../OpenRGBEffectsPlugin/ZoneListItem.ui                                                     \
    ../OpenRGBEffectsPlugin/GlobalSettings.ui                                                   \

#-----------------------------------------------------------------------------------------------#
# OpenRGB Visual Map Plugin - Statically Linked                                                 #
# NOTE: Placed AFTER OpenRGBEffectsPlugin in  build order, but has priority include path       #
#       for ControllerZone.h to resolve naming conflict                                         #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    ../OpenRGBVisualMapPlugin                                                                   \
    ../OpenRGBVisualMapPlugin/ui                                                                \
    ../OpenRGBVisualMapPlugin/ui/widget-editor                                                  \

HEADERS +=                                                                                      \
    ../OpenRGBVisualMapPlugin/OpenRGBPluginsFont.h                                              \
    ../OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.h                                          \
    ../OpenRGBVisualMapPlugin/ZoneManager.h                                                     \
    ../OpenRGBVisualMapPlugin/VirtualController.h                                               \
    ../OpenRGBVisualMapPlugin/VisualMapSettingsManager.h                                        \
    ../OpenRGBVisualMapPlugin/VisualMapJsonDefinitions.h                                        \
    ../OpenRGBVisualMapPlugin/VisualMapControllerZone.h                                        \
    ../OpenRGBVisualMapPlugin/ui/ClickableLabel.h                                               \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.h                                                  \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.h                                                    \
    ../OpenRGBVisualMapPlugin/ui/ControllerZoneItem.h                                           \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.h                                                   \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/EditableLabel.h                                                \
    ../OpenRGBVisualMapPlugin/ui/GradientPresets.h                                              \
    ../OpenRGBVisualMapPlugin/ui/GridSettings.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.h                                                   \
    ../OpenRGBVisualMapPlugin/ui/Scene.h                                                        \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.h                                                    \
    ../OpenRGBVisualMapPlugin/ui/TooltipProxy.h                                                 \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.h                                         \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/EditorGrid.h                                     \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/LedItem.h                                        \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.h                                            \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.h                                          \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.h                                   \
    ../OpenRGBVisualMapPlugin/ui/Grid.h                                                         \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.h                                                  \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.h                                                  \

SOURCES +=                                                                                      \
    ../OpenRGBVisualMapPlugin/OpenRGBPluginsFont.cpp                                            \
    ../OpenRGBVisualMapPlugin/OpenRGBVisualMapPlugin.cpp                                        \
    ../OpenRGBVisualMapPlugin/VirtualController.cpp                                             \
    ../OpenRGBVisualMapPlugin/VisualMapSettingsManager.cpp                                      \
    ../OpenRGBVisualMapPlugin/ZoneManager.cpp                                                   \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.cpp                                          \
    ../OpenRGBVisualMapPlugin/ui/ClickableLabel.cpp                                             \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.cpp                                                  \
    ../OpenRGBVisualMapPlugin/ui/ControllerZoneItem.cpp                                         \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.cpp                                                 \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.cpp                                               \
    ../OpenRGBVisualMapPlugin/ui/EditableLabel.cpp                                              \
    ../OpenRGBVisualMapPlugin/ui/Grid.cpp                                                       \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.cpp                                                \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.cpp                                        \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.cpp                                                 \
    ../OpenRGBVisualMapPlugin/ui/Scene.cpp                                                      \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.cpp                                                  \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.cpp                                       \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/EditorGrid.cpp                                   \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/LedItem.cpp                                      \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.cpp                                 \

FORMS +=                                                                                        \
    ../OpenRGBVisualMapPlugin/ui/BackgroundApplier.ui                                           \
    ../OpenRGBVisualMapPlugin/ui/ColorPicker.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/ColorStop.ui                                                   \
    ../OpenRGBVisualMapPlugin/ui/DeviceList.ui                                                  \
    ../OpenRGBVisualMapPlugin/ui/DeviceWidget.ui                                                \
    ../OpenRGBVisualMapPlugin/ui/GridOptions.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/ItemOptions.ui                                                 \
    ../OpenRGBVisualMapPlugin/ui/OpenRGBVisualMapTab.ui                                         \
    ../OpenRGBVisualMapPlugin/ui/PluginInfo.ui                                                  \
    ../OpenRGBVisualMapPlugin/ui/TabHeader.ui                                                   \
    ../OpenRGBVisualMapPlugin/ui/VirtualControllerTab.ui                                        \
    ../OpenRGBVisualMapPlugin/ui/widget-editor/WidgetEditor.ui                                  \

#-----------------------------------------------------------------------------------------------#
# Windows-specific plugin sources                                                               #
#-----------------------------------------------------------------------------------------------#
win32:SOURCES +=                                                                                \
    ../OpenRGBEffectsPlugin/ScreenCapturer/windows/WindowsScreenCapturer.cpp                    \

win32:HEADERS +=                                                                                \
    ../OpenRGBEffectsPlugin/ScreenCapturer/windows/WindowsScreenCapturer.h                      \

win32:LIBS +=                                                                                   \
    -lOpenGL32                                                                                  \
    -lgdi32                                                                                     \
    -luser32                                                                                    \

#-----------------------------------------------------------------------------------------------#
# Linux-specific plugin sources                                                                 #
#-----------------------------------------------------------------------------------------------#
unix:!macx {
    INCLUDEPATH +=                                                                              \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland                                    \
        /usr/include/pipewire-0.3                                                               \
        /usr/include/spa-0.2                                                                    \

    SOURCES +=                                                                                  \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/DBusScreenCastManager.cpp          \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/PipeWireCapturer.cpp               \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/SpaPodUtils.cpp                    \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/PipeWireStreamInfo.cpp             \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/WaylandScreenCapturer.cpp          \

    HEADERS +=                                                                                  \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/DBusScreenCastManager.h            \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/PipeWireCapturer.h                 \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/SpaPodUtils.h                      \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/PipeWireStreamInfo.h               \
        ../OpenRGBEffectsPlugin/ScreenCapturer/linux/wayland/WaylandScreenCapturer.h            \

    LIBS +=                                                                                     \
        -lopenal                                                                                \
        -lGL                                                                                    \
        -lpipewire-0.3                                                                          \
}

#-----------------------------------------------------------------------------------------------#
# macOS-specific plugin sources                                                                 #
#-----------------------------------------------------------------------------------------------#
macx: {
    LIBS += -framework OpenAL
}

RESOURCES +=                                                                                    \
    ../OpenRGBEffectsPlugin/resources.qrc                                                       \
    ../OpenRGBVisualMapPlugin/resources.qrc                                                     \

