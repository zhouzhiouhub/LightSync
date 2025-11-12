#-----------------------------------------------------------------------------------------------#
# LightSync unified qmake project                                                               #
#   Builds OpenRGB app and Effects plugin together                                               #
#-----------------------------------------------------------------------------------------------#

TEMPLATE = subdirs

SUBDIRS += \
    OpenRGB \
    OpenRGBEffectsPlugin \
    OpenRGBVisualMapPlugin

# Ensure OpenRGB builds before plugin so headers/libs are available
OpenRGBEffectsPlugin.depends = OpenRGB
OpenRGBVisualMapPlugin.depends = OpenRGB


