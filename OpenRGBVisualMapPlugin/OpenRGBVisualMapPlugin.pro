#-----------------------------------------------------------------------------------------------#
# OpenRGB Visual Map Plugin QMake Project                                                       #
#-----------------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------------#
# Qt Configuration                                                                              #
#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \
    widgets

DEFINES += ORGBVISUALMAPPLUGIN_LIBRARY \
    OPEN_RGB_VISUALMAP_PLUGIN_LIBRARY
TEMPLATE = lib
TARGET = OpenRGBVisualMapPlugin

#-----------------------------------------------------------------------------------------------#
# Build Configuration                                                                           #
#-----------------------------------------------------------------------------------------------#
CONFIG +=                                                                                       \
    plugin                                                                                      \
    silent

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
MAJOR       = 0
MINOR       = 9
SUFFIX      = git

SHORTHASH   = $$system("git rev-parse --short=7 HEAD")
COMMITS     = $$system("git rev-list --count HEAD")

VERSION_NUM = $$MAJOR"."$$MINOR"."$$COMMITS
VERSION_STR = $$MAJOR"."$$MINOR

VERSION_DEB = $$VERSION_NUM
VERSION_WIX = $$VERSION_NUM
VERSION_AUR = $$VERSION_NUM
VERSION_RPM = $$VERSION_NUM

equals(SUFFIX, "git") {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX$$COMMITS")"
VERSION_DEB = $$VERSION_DEB"~git"$$SHORTHASH
VERSION_AUR = $$VERSION_AUR".g"$$SHORTHASH
VERSION_RPM = $$VERSION_RPM"^git"$$SHORTHASH
} else {
    !isEmpty(SUFFIX) {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX")"
VERSION_DEB = $$VERSION_DEB"~"$$SUFFIX
VERSION_AUR = $$VERSION_AUR"."$$SUFFIX
VERSION_RPM = $$VERSION_RPM"^"$$SUFFIX
    }
}

message("VERSION_NUM: "$$VERSION_NUM)
message("VERSION_STR: "$$VERSION_STR)
message("VERSION_DEB: "$$VERSION_DEB)
message("VERSION_WIX: "$$VERSION_WIX)
message("VERSION_AUR: "$$VERSION_AUR)
message("VERSION_RPM: "$$VERSION_RPM)

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git log -n 1 --pretty=format:"%H")
GIT_COMMIT_DATE = $$system(git log -n 1 --pretty=format:"%ci")
GIT_BRANCH      = $$system(git branch --show-current)

#-----------------------------------------------------------------------------------------------#
# Download links                                                                                #
#-----------------------------------------------------------------------------------------------#
win32:VISUALMAP_LATEST_BUILD_URL="https://gitlab.com/OpenRGBDevelopers/openrgbvisualmapplugin/-/jobs/artifacts/master/download?job=Windows 64"
unix:!macx:VISUALMAP_LATEST_BUILD_URL="https://gitlab.com/OpenRGBDevelopers/openrgbvisualmapplugin/-/jobs/artifacts/master/download?job=Linux 64"

#-----------------------------------------------------------------------------------------------#
# Inject vars in defines                                                                        #
#-----------------------------------------------------------------------------------------------#
DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$VERSION_STR\\"\"\"                                                  \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"                                                       \
    VISUALMAP_LATEST_BUILD_URL=\\"\"\"$$VISUALMAP_LATEST_BUILD_URL\\"\"\"                       \

#-----------------------------------------------------------------------------------------------#
# OpenRGB Plugin SDK                                                                            #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    ../OpenRGB                                                                                  \
    ../OpenRGB/i2c_smbus                                                                        \
    ../OpenRGB/RGBController                                                                    \
    ../OpenRGB/net_port                                                                         \
    ../OpenRGB/dependencies/json                                                                \
    ../OpenRGB/qt                                                                               \
    ../OpenRGB/SPDAccessor                                                                      \
    ../OpenRGB/hidapi_wrapper                                                                  \

HEADERS +=                                                                                      \
    ../OpenRGB/Colors.h                                                                         \
    ../OpenRGB/OpenRGBPluginInterface.h                                                         \
    ../OpenRGB/ResourceManagerInterface.h                                                       \

SOURCES +=                                                                                      \
    ../OpenRGB/RGBController/RGBController.cpp                                                  \
    ../OpenRGB/RGBController/RGBController_Network.cpp                                          \
    ../OpenRGB/NetworkServer.cpp                                                                \
    ../OpenRGB/NetworkClient.cpp                                                                \
    ../OpenRGB/NetworkProtocol.cpp                                                              \
    ../OpenRGB/LogManager.cpp                                                                   \
    ../OpenRGB/net_port/net_port.cpp                                                            \
    ../OpenRGB/qt/hsv.cpp

#-------------------------------------------------------------------#
# Includes                                                          #
#-------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    ui                                                                                          \
    ui/widget-editor                                                                            \

HEADERS +=                                                                                      \
    OpenRGBPluginsFont.h                                                                        \
    OpenRGBVisualMapPlugin.h                                                                    \
    ZoneManager.h                                                                               \
    VirtualController.h                                                                         \
    VisualMapSettingsManager.h                                                                  \
    VisualMapJsonDefinitions.h                                                                  \
    VisualMapControllerZone.h                                                                  \
    ui/ClickableLabel.h                                                                         \
    ui/ColorPicker.h                                                                            \
    ui/ColorStop.h                                                                              \
    ui/ControllerZoneItem.h                                                                     \
    ui/DeviceList.h                                                                             \
    ui/DeviceWidget.h                                                                           \
    ui/EditableLabel.h                                                                          \
    ui/GradientPresets.h                                                                        \
    ui/GridSettings.h                                                                           \
    ui/PluginInfo.h                                                                             \
    ui/Scene.h                                                                                  \
    ui/TabHeader.h                                                                              \
    ui/TooltipProxy.h                                                                           \
    ui/VirtualControllerTab.h                                                                   \
    ui/widget-editor/EditorGrid.h                                                               \
    ui/widget-editor/LedItem.h                                                                  \
    ui/BackgroundApplier.h                                                                      \
    ui/OpenRGBVisualMapTab.h                                                                    \
    ui/widget-editor/WidgetEditor.h                                                             \
    ui/Grid.h                                                                                   \
    ui/GridOptions.h                                                                            \
    ui/ItemOptions.h                                                                            \

SOURCES +=                                                                                      \
    OpenRGBPluginsFont.cpp                                                                      \
    OpenRGBVisualMapPlugin.cpp                                                                  \
    VirtualController.cpp                                                                       \
    VisualMapSettingsManager.cpp                                                                \
    ZoneManager.cpp                                                                             \
    ui/BackgroundApplier.cpp                                                                    \
    ui/ClickableLabel.cpp                                                                       \
    ui/ColorPicker.cpp                                                                          \
    ui/ColorStop.cpp                                                                            \
    ui/ControllerZoneItem.cpp                                                                   \
    ui/DeviceList.cpp                                                                           \
    ui/DeviceWidget.cpp                                                                         \
    ui/EditableLabel.cpp                                                                        \
    ui/Grid.cpp                                                                                 \
    ui/GridOptions.cpp                                                                          \
    ui/ItemOptions.cpp                                                                          \
    ui/OpenRGBVisualMapTab.cpp                                                                  \
    ui/PluginInfo.cpp                                                                           \
    ui/Scene.cpp                                                                                \
    ui/TabHeader.cpp                                                                            \
    ui/VirtualControllerTab.cpp                                                                 \
    ui/widget-editor/EditorGrid.cpp                                                             \
    ui/widget-editor/LedItem.cpp                                                                \
    ui/widget-editor/WidgetEditor.cpp                                                           \

FORMS +=                                                                                        \
    ui/BackgroundApplier.ui                                                                     \
    ui/ColorPicker.ui                                                                           \
    ui/ColorStop.ui                                                                             \
    ui/DeviceList.ui                                                                            \
    ui/DeviceWidget.ui                                                                          \
    ui/GridOptions.ui                                                                           \
    ui/ItemOptions.ui                                                                           \
    ui/OpenRGBVisualMapTab.ui                                                                   \
    ui/PluginInfo.ui                                                                            \
    ui/TabHeader.ui                                                                             \
    ui/VirtualControllerTab.ui                                                                  \
    ui/widget-editor/WidgetEditor.ui                                                            \

#-----------------------------------------------------------------------------------------------#
# Windows-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
win32:CONFIG += QTPLUGIN c++17
win32:BUILD_BASE_DIR = $$PWD/../build/LightSync

win32:CONFIG(debug, debug|release) {
    BIN_SUBDIR = debug
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}

win32:CONFIG(release, debug|release) {
    BIN_SUBDIR = release
    win32:DESTDIR = $$PWD/../out/LightSync/$$BIN_SUBDIR/plugins
}

win32:OBJECTS_DIR = $$BUILD_BASE_DIR/visualmap/_intermediate_$$BIN_SUBDIR/.obj
win32:MOC_DIR     = $$BUILD_BASE_DIR/visualmap/_intermediate_$$BIN_SUBDIR/.moc
win32:RCC_DIR     = $$BUILD_BASE_DIR/visualmap/_intermediate_$$BIN_SUBDIR/.qrc
win32:UI_DIR      = $$BUILD_BASE_DIR/visualmap/_intermediate_$$BIN_SUBDIR/.ui
win32:INCLUDEPATH +=                                                                          \
    ../OpenRGB/dependencies/hidapi-win/include

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -lole32                                                                                 \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -lole32                                                                                 \
}

win32:DEFINES +=                                                                                \
    _MBCS                                                                                       \
    WIN32                                                                                       \
    _CRT_SECURE_NO_WARNINGS                                                                     \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                                             \
    WIN32_LEAN_AND_MEAN                                                                         \

#-----------------------------------------------------------------------------------------------#
# Linux-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
unix:!macx {
    QMAKE_CXXFLAGS += -std=c++17 -Wno-psabi
    target.path=$$PREFIX/lib/openrgb/plugins/
    INSTALLS += target
}

#-----------------------------------------------------------------------------------------------#
# MacOS-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

macx: {
    CONFIG += c++17
}

RESOURCES +=                                                                                    \
    resources.qrc
