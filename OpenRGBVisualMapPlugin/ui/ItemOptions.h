#ifndef ITEMOPTIONS_H
#define ITEMOPTIONS_H

#include <QWidget>
#include "VisualMapControllerZone.h"

namespace Ui {
class ItemOptions;
}

class ItemOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ItemOptions(QWidget *parent = nullptr);
    ~ItemOptions();

    void SetControllerZone(VisualMapControllerZone*);
    void Update();

signals:
    void ItemOptionsChanged() const;
    void ShapeEditRequest(VisualMapControllerZone*);

private:
    Ui::ItemOptions *ui;
    VisualMapControllerZone* ctrl_zone = nullptr;

    void UpdateWidgetsVisibility();

private slots:
    void on_x_spinBox_valueChanged(int);
    void on_y_spinBox_valueChanged(int);
    void on_led_spacing_spinBox_valueChanged(int);
    void on_shape_comboBox_currentIndexChanged(int);
    void on_reverse_checkBox_stateChanged(int);
    void on_identifyButton_clicked();
    void on_edit_shape_button_clicked();

};

#endif // ITEMOPTIONS_H
