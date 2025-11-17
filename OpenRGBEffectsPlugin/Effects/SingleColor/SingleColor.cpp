#include "SingleColor.h"

REGISTER_EFFECT(SingleColor);

SingleColor::SingleColor(QWidget *parent) :
	RGBEffect(parent),
	ui(new Ui::SingleColor)
{
	ui->setupUi(this);

	SetDynamicStrings();
	EffectDetails.EffectClassName = ClassName();
	EffectDetails.UserColors      = 1;
	EffectDetails.SupportsRandom  = false;
}

SingleColor::~SingleColor()
{
	delete ui;
}

void SingleColor::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		SetDynamicStrings();
	}
}

void SingleColor::SetDynamicStrings()
{
	EffectDetails.EffectName        = tr(UI_Name().c_str()).toStdString();
	EffectDetails.EffectDescription = tr("Set all selected devices to a single RGB color").toStdString();
}

void SingleColor::StepEffect(std::vector<ControllerZone*> controller_zones)
{
	if(EffectDetails.UserColors == 0 || UserColors.empty())
	{
		return;
	}

	RGBColor color = UserColors[0];

	for(ControllerZone* controller_zone : controller_zones)
	{
		controller_zone->SetAllZoneLEDs(color, Brightness, Temperature, Tint);
	}
}


