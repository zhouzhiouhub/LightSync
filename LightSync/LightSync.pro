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
    $$PWD/../OpenRGB/dependencies/json

#-----------------------------------------------------------------------------------------------#
# Global defines                                                                                 #
#-----------------------------------------------------------------------------------------------#
# Provide defaults for OpenRGB LogManager expected git macros
DEFINES += GIT_COMMIT_ID=\"local\" GIT_COMMIT_DATE=\"unknown\"

#-----------------------------------------------------------------------------------------------#
# Sources / Headers                                                                               #
#-----------------------------------------------------------------------------------------------#
SOURCES +=                                                                                        \
    $$PWD/app/main.cpp                                                                            \
    $$PWD/ui/MainWindow.cpp                                                                       \
    $$PWD/../OpenRGB/LogManager.cpp

HEADERS +=                                                                                        \
    $$PWD/ui/MainWindow.h                                                                         \
    $$PWD/core/models/Models.h                                                                     \
    $$PWD/core/effects/Effect.h                                                                    \
    $$PWD/core/effects/ScreenSyncEffect.h                                                          \
    $$PWD/core/effects/MusicEffect.h                                                               \
    $$PWD/core/services/Services.h
