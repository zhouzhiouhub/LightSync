#ifndef ZONEMANAGER_H
#define ZONEMANAGER_H

#include <QImage>
#include <vector>
#include "VisualMapControllerZone.h"

class ZoneManager
{
public:
    static ZoneManager* Get();

    std::vector<VisualMapControllerZone*> GetAvailableZones();

    void IdentifyZone(VisualMapControllerZone*);
    void IdentifyLeds(VisualMapControllerZone*, std::vector<unsigned int>);

private:
    ZoneManager(){};
    static ZoneManager* instance;

    void SetControllerZoneColor(VisualMapControllerZone*, QColor);
    void InitMatrixCustomShape(VisualMapControllerZone*);
};

#endif // ZONEMANAGER_H
