#ifndef ZIGZAG_H
#define ZIGZAG_H

#include <QWidget>
#include "ui_ZigZag.h"
#include "RGBEffect.h"
#include "EffectRegisterer.h"

namespace Ui {
class ZigZag;
}

class ZigZag : public RGBEffect
{
    Q_OBJECT

public:
    explicit ZigZag(QWidget *parent = nullptr);
    ~ZigZag();

    EFFECT_REGISTERER(ClassName(), UI_Name(), CAT_ADVANCED, [](){return new ZigZag;});

    static std::string const ClassName() {return "ZigZag";}
    static std::string const UI_Name() { return QT_TR_NOOP("ZigZag"); }

    void StepEffect(std::vector<ControllerZone*>) override;

private:
    Ui::ZigZag *ui;

    void SetDynamicStrings();

    RGBColor GetColor(float,float,float,float);

    double time = 0.;
    double progress = 0.;

private slots:
    void changeEvent(QEvent *event) override;
};

#endif // ZIGZAG_H
