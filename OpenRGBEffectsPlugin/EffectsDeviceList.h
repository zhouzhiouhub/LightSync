#ifndef EFFECTSDEVICELIST_H
#define EFFECTSDEVICELIST_H

#include <QWidget>
#include "ControllerZone.h"
#include "DeviceListItem.h"

namespace Ui {
class EffectsDeviceList;
}

class EffectsDeviceList : public QWidget
{
    Q_OBJECT

public:
    explicit EffectsDeviceList(QWidget *parent = nullptr);
    ~EffectsDeviceList();

    void Clear();
    void InitControllersList();

    void DisableControls();
    void EnableControls();

    void ApplySelection(std::vector<ControllerZone*>);
    std::vector<ControllerZone*> GetSelection();
    std::vector<ControllerZone*> GetControllerZones();

signals:
    void SelectionChanged();

private slots:
    void changeEvent(QEvent *event) override;
    void on_toggle_select_all_clicked();
    void on_toggle_reverse_clicked();
    void on_toggle_brightness_clicked();

private:
    Ui::EffectsDeviceList *ui;

    std::vector<DeviceListItem*> device_items;
    std::vector<ControllerZone*> controller_zones;
};

#endif // EFFECTSDEVICELIST_H
