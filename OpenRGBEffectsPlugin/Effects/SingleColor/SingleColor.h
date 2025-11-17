#ifndef SINGLECOLOR_H
#define SINGLECOLOR_H

#include <QWidget>
#include "ui_SingleColor.h"
#include "RGBEffect.h"
#include "EffectRegisterer.h"

namespace Ui {
class SingleColor;
}

class SingleColor : public RGBEffect
{
	Q_OBJECT

public:
	explicit SingleColor(QWidget *parent = nullptr);
	~SingleColor();

	EFFECT_REGISTERER(ClassName(), UI_Name(), CAT_SIMPLE, [](){return new SingleColor;});

	static std::string const ClassName() { return "SingleColor"; }
	static std::string const UI_Name() { return QT_TR_NOOP("Single Color"); }

	void StepEffect(std::vector<ControllerZone*>) override;

private:
	Ui::SingleColor *ui;

	void SetDynamicStrings();

private slots:
	void changeEvent(QEvent *event) override;
};

#endif // SINGLECOLOR_H


