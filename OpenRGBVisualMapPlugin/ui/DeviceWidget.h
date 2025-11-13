#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include "VisualMapControllerZone.h"

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(QWidget *parent = nullptr, VisualMapControllerZone* controller_zone = nullptr, bool in_group = false);
    ~DeviceWidget();

    VisualMapControllerZone* getControllerZone();
    void setSelected(bool);
    bool isSelected();

    void setEnabled(bool);
    bool isEnabled();
    void updateName();

private slots:
    void on_enable_toggled(bool);
    void on_select_toggled(bool);
    void on_rename_clicked();
    void on_name_clicked();

signals:
    void Enabled(bool);
    void Selected(bool);
    void Renamed(QString);

private:
    Ui::DeviceWidget *ui;
    VisualMapControllerZone* controller_zone;
    bool in_group;
    void UpdateCheckState();
};

#endif // DEVICEWIDGET_H
