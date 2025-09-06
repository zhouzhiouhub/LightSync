#ifndef SPIRAL_H
#define SPIRAL_H

#include <QWidget>
#include "ui_Spiral.h"
#include "RGBEffect.h"
#include "EffectRegisterer.h"

namespace Ui {
class Spiral;
}

class Spiral : public RGBEffect
{
    Q_OBJECT

public:
    explicit Spiral(QWidget *parent = nullptr);
    ~Spiral();

    EFFECT_REGISTERER(ClassName(), UI_Name(), CAT_ADVANCED, [](){return new Spiral;});

    static std::string const ClassName() {return "Spiral";}
    static std::string const UI_Name() { return QT_TR_NOOP("Spiral"); }

    void StepEffect(std::vector<ControllerZone*>) override;

private:
    Ui::Spiral *ui;

    void SetDynamicStrings();

    double time = 1000.0;

    RGBColor GetColor(float, float, float, float, bool);

private slots:
    void changeEvent(QEvent *event) override;
};

#endif // SPIRAL_H
