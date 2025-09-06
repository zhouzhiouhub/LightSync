#ifndef MOVINGPANES_H
#define MOVINGPANES_H

#include <QWidget>
#include "ui_MovingPanes.h"
#include "RGBEffect.h"
#include "EffectRegisterer.h"

namespace Ui {
class MovingPanes;
}

class MovingPanes : public RGBEffect
{
    Q_OBJECT

public:
    explicit MovingPanes(QWidget *parent = nullptr);
    ~MovingPanes();

    EFFECT_REGISTERER(ClassName(), UI_Name(), CAT_ADVANCED, [](){return new MovingPanes;});

    static std::string const ClassName() {return "MovingPanes";}
    static std::string const UI_Name() { return QT_TR_NOOP("Moving Panes"); }

    void StepEffect(std::vector<ControllerZone*>) override;

private:
    Ui::MovingPanes *ui;

    void SetDynamicStrings();

    double time = 0.0;
    RGBColor GetColor(float, float, float, float, bool);

private slots:
    void changeEvent(QEvent *event) override;
};

#endif // MOVINGPANES_H
