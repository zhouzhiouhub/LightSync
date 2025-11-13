#-----------------------------------------------------------------------------------------------#
# LightSync unified qmake project                                                               #
#   Builds OpenRGB app with integrated plugins (no longer separate DLLs)                        #
#-----------------------------------------------------------------------------------------------#

TEMPLATE = subdirs

SUBDIRS += \
    OpenRGB

# Plugins are now statically compiled into OpenRGB executable
# OpenRGBEffectsPlugin and OpenRGBVisualMapPlugin source is included via plugins_static.pri



