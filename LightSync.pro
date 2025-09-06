#-----------------------------------------------------------------------------------------------#
# LightSync unified qmake project                                                               #
#   Builds OpenRGB app and Effects plugin together                                               #
#-----------------------------------------------------------------------------------------------#

TEMPLATE = subdirs

SUBDIRS += \
    OpenRGB \
    OpenRGBEffectsPlugin

# Ensure OpenRGB builds before plugin so headers/libs are available
OpenRGBEffectsPlugin.depends = OpenRGB


