#include "OpenRGBPluginsFont.h"
#include <QFontDatabase>
#include <QList>
#include <QString>

OpenRGBPluginsFont* OpenRGBPluginsFont::instance;

OpenRGBPluginsFont::OpenRGBPluginsFont(){}

OpenRGBPluginsFont *OpenRGBPluginsFont::Get()
{
    if(!instance)
    {
        instance = new OpenRGBPluginsFont();
        // Load plugin-scoped icon font from resources. Path must match alias in resources.qrc
        // OpenRGBEffectsPlugin/resources.qrc: <file alias="effects/OpenRGBPlugins.ttf">OpenRGBPlugins.ttf</file>
        instance->fontId = QFontDatabase::addApplicationFont(":/effects/OpenRGBPlugins.ttf");

        if(instance->fontId == -1)
        {
            printf("Cannot load requested font.\n");
        }
        else
        {
            QString family = QFontDatabase::applicationFontFamilies(instance->fontId).at(0);
            instance->font = QFont(family, 13);
            instance->font.setStyleStrategy(QFont::PreferAntialias);
        }
    }

    return instance;
}

QString OpenRGBPluginsFont::icon(int glyph)
{
    return QChar(glyph);
}

QFont OpenRGBPluginsFont::GetFont()
{
    return Get()->font;
}

