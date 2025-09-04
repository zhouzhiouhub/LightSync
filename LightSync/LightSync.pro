#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \
    widgets                                                                                     \
    network                                                                                     \
    concurrent                                                                                  \
    svg                                                                                         \
    serialport                                                                                  \
    dbus
if(greaterThan(QT_MAJOR_VERSION, 5)) {
QT +=                                                                                           \
    core5compat                                                                                 \
}

#-----------------------------------------------------------------------------------------------#
# Set compiler to use C++17 to make std::filesystem available                                   #
#-----------------------------------------------------------------------------------------------#
CONFIG +=   c++17                                                                               \
            lrelease                                                                            \
            embed_translations                                                                  \
            silent                                                                             \
            force_debug_info                                                                     \

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
TEMPLATE    = app
TARGET      = LightSync

#-----------------------------------------------------------------------------------------------#
# Include paths                                                                                  #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                   \
    $$PWD                                                                                         \
    $$PWD/../OpenRGB                                                                              \
    $$PWD/../OpenRGB/dependencies/json                                                            \
    $$PWD/../OpenRGB/net_port                                                                     \
    $$PWD/../OpenRGB/hidapi_wrapper                                                               \
    $$PWD/../OpenRGB/i2c_smbus                                                                    \
    $$PWD/../OpenRGB/Controllers                                                                  \
    $$PWD/../OpenRGB/RGBController

#-----------------------------------------------------------------------------------------------#
# Global defines                                                                                 #
#-----------------------------------------------------------------------------------------------#
# Provide defaults for OpenRGB LogManager expected git macros
DEFINES += GIT_COMMIT_ID=\"local\" GIT_COMMIT_DATE=\"unknown\" NOMINMAX

# On Windows, force ANSI WinAPI for OpenRGB serial/SetupAPI code
win32:DEFINES -= UNICODE _UNICODE
win32-msvc:QMAKE_CXXFLAGS += /UUNICODE /U_UNICODE
win32-msvc:QMAKE_CFLAGS  += /UUNICODE /U_UNICODE
win32-g++:QMAKE_CXXFLAGS += -UUNICODE -U_UNICODE
win32-g++:QMAKE_CFLAGS  += -UUNICODE -U_UNICODE

#-----------------------------------------------------------------------------------------------#
# Sources / Headers                                                                               #
#-----------------------------------------------------------------------------------------------#
SOURCES +=                                                                                        \
    $$PWD/app/main.cpp                                                                            \
    $$PWD/ui/MainWindow.cpp                                                                       \
    $$PWD/ui/pages/HomePage.cpp                                                                    \
    $$PWD/ui/pages/DeviceControlPage.cpp                                                           \
    $$PWD/ui/widgets/DeviceCard.cpp                                                                \
    $$PWD/ui/widgets/LedEditorDialog.cpp                                                           \
    $$PWD/cli_stub.cpp                                                                             \
    $$PWD/../OpenRGB/LogManager.cpp                                                                \
    $$PWD/../OpenRGB/ResourceManager.cpp                                                           \
    $$PWD/../OpenRGB/SettingsManager.cpp                                                           \
    $$PWD/../OpenRGB/ProfileManager.cpp                                                            \
    $$PWD/../OpenRGB/StringUtils.cpp                                                               \
    $$PWD/../OpenRGB/net_port/net_port.cpp                                                         \
    $$PWD/../OpenRGB/NetworkClient.cpp                                                             \
    $$PWD/../OpenRGB/NetworkProtocol.cpp                                                           \
    $$PWD/../OpenRGB/NetworkServer.cpp                                                             \
    $$PWD/../OpenRGB/RGBController/RGBController.cpp                                               \
    $$PWD/../OpenRGB/RGBController/RGBController_Network.cpp                                       \
    $$PWD/../OpenRGB/RGBController/RGBController_Dummy.cpp

HEADERS +=                                                                                        \
    $$PWD/ui/MainWindow.h                                                                         \
    $$PWD/ui/pages/HomePage.h                                                                      \
    $$PWD/ui/pages/DeviceControlPage.h                                                             \
    $$PWD/ui/widgets/DeviceCard.h                                                                  \
    $$PWD/ui/widgets/LedEditorDialog.h                                                             \
    $$PWD/core/models/Models.h                                                                     \
    $$PWD/core/effects/Effect.h                                                                    \
    $$PWD/core/effects/ScreenSyncEffect.h                                                          \
    $$PWD/core/effects/MusicEffect.h                                                               \
    $$PWD/core/services/Services.h                                                                 \
    $$PWD/../OpenRGB/ResourceManager.h                                                             \
    $$PWD/../OpenRGB/ResourceManagerInterface.h                                                    \
    $$PWD/../OpenRGB/SettingsManager.h                                                             \
    $$PWD/../OpenRGB/ProfileManager.h                                                              \
    $$PWD/../OpenRGB/RGBController/RGBController.h

# NVAPI include path for i2c_smbus_nvapi
win32:INCLUDEPATH += $$PWD/../OpenRGB/dependencies/NVFC

# Link dependencies for OpenRGB core
win32:LIBS += -lws2_32
win32:INCLUDEPATH += $$PWD/../OpenRGB/dependencies/hidapi-win/include
win32:LIBS += -L"$$PWD/../OpenRGB/dependencies/hidapi-win/x64/" -lhidapi
win32:SOURCES += $$PWD/../OpenRGB/dependencies/NVFC/nvapi.cpp
win32:LIBS += -lsetupapi -ladvapi32

# Include a conservative subset of platform backends needed by ResourceManager
INCLUDEPATH += $$PWD/../OpenRGB/SPDAccessor
win32:SOURCES +=                                                                                   \
    $$PWD/../OpenRGB/i2c_smbus/i2c_smbus_nvapi.cpp                                                 \
    $$PWD/../OpenRGB/i2c_smbus/i2c_smbus.cpp                                                       \
    # $$PWD/../OpenRGB/i2c_smbus/i2c_smbus_amdadl.cpp  # requires AMD ADL SDK (adl_sdk.h); omit for now \
    $$PWD/../OpenRGB/serial_port/serial_port.cpp                                                   \
    $$PWD/../OpenRGB/serial_port/find_usb_serial_port_win.cpp

# SPD Accessor sources for DIMM detection and SPDWrapper
SOURCES +=                                                                                        \
    $$PWD/../OpenRGB/SPDAccessor/SPDAccessor.cpp                                                   \
    $$PWD/../OpenRGB/SPDAccessor/SPDWrapper.cpp                                                    \
    $$PWD/../OpenRGB/SPDAccessor/SPDDetector.cpp                                                   \
    $$PWD/../OpenRGB/SPDAccessor/DDR4DirectAccessor.cpp                                            \
    $$PWD/../OpenRGB/SPDAccessor/DDR5DirectAccessor.cpp

# Exclude OpenRGB GUI/Qt pages to avoid duplicate UI; LightSync supplies UI
DEFINES += OPENRGB_CORE_ONLY
win32-msvc:QMAKE_CXXFLAGS += /Uinterface
win32-msvc:QMAKE_CFLAGS  += /Uinterface
win32-g++:QMAKE_CXXFLAGS += -Uinterface
win32-g++:QMAKE_CFLAGS  += -Uinterface
