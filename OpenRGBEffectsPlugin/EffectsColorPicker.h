#ifndef EFFECTS_COLORPICKER_H
#define EFFECTS_COLORPICKER_H

#include <QWidget>
#include "RGBController.h"
#include "ColorUtils.h"

namespace Ui {
class EffectsColorPicker;
}

class EffectsColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit EffectsColorPicker(QWidget *parent = nullptr);
    ~EffectsColorPicker();

    RGBColor CurrentRGBColor();
    QColor CurrentQColor();

    void SetQColor(QColor);
    void SetRGBColor(RGBColor);

private slots:
    void changeEvent(QEvent *event) override;
    void on_button_clicked();

signals:
    void ColorSelected(QColor);

private:
    Ui::EffectsColorPicker *ui;
    QColor current_color;
};

#endif // EFFECTS_COLORPICKER_H
