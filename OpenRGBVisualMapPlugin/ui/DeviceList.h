#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QWidget>
#include "VisualMapControllerZone.h"
#include "DeviceWidget.h"

namespace Ui {
class DeviceList;
}

class DeviceList : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceList(QWidget *parent = nullptr);
    ~DeviceList();

    void Clear();
    void Init(std::vector<VisualMapControllerZone*>);
    void SetSelection(std::vector<VisualMapControllerZone*>);
    void UpdateControllerState(VisualMapControllerZone*);

signals:
    void DeviceAdded(VisualMapControllerZone*);
    void DeviceRemoved(VisualMapControllerZone*);
    void SelectionChanged(std::vector<VisualMapControllerZone*>);

private:
    Ui::DeviceList *ui;
    std::vector<DeviceWidget*> device_widgets;
};

#endif // DEVICELIST_H
