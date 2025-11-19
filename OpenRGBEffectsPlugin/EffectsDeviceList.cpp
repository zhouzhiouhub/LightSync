#include "EffectsDeviceList.h"
#include "ui_EffectsDeviceList.h"
#include "OpenRGBPluginsFont.h"

#include "OpenRGBEffectsPlugin.h"
#include "OpenRGBEffectSettings.h"
#include <QVBoxLayout>


EffectsDeviceList::EffectsDeviceList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectsDeviceList)
{
    ui->setupUi(this);
    ui->devices->setLayout(new QVBoxLayout(ui->devices));

    ApplyIconFonts();

    InitControllersList();
}

EffectsDeviceList::~EffectsDeviceList()
{
    delete ui;
}

void EffectsDeviceList::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        ApplyIconFonts();
    }
}

void EffectsDeviceList::Clear()
{
    device_items.clear();

    QLayoutItem *child;

    while ((child = ui->devices->layout()->takeAt(0)) != 0)
    {
        delete child->widget();
    }
}

void EffectsDeviceList::InitControllersList()
{
    std::vector<RGBController*> controllers = OpenRGBEffectsPlugin::RMPointer->GetRGBControllers();

    controller_zones.clear();

    for(RGBController* controller : controllers)
    {
        bool has_direct = false;

        for(unsigned int i = 0; i < controller->modes.size(); i++)
        {
            if(controller->modes[i].name == "Direct")
            {
                has_direct = true;
                break;
            }
        }

        if(OpenRGBEffectSettings::globalSettings.hide_unsupported && !has_direct)
        {
            continue;
        }

        std::vector<ControllerZone*> iteration_zones;

        for(unsigned int i = 0; i < controller->zones.size(); i++)
        {
            ControllerZone* controller_zone = new ControllerZone(controller, i, false, 100, false);
            controller_zones.push_back(controller_zone);
            iteration_zones.push_back(controller_zone);

            for(unsigned int s = 0; s <  controller->zones[i].segments.size(); s++)
            {
                ControllerZone* controller_zone = new ControllerZone(controller, i, false, 100, true, s);
                controller_zones.push_back(controller_zone);
                iteration_zones.push_back(controller_zone);
            }

        }

        if(iteration_zones.empty())
        {
            continue;
        }

        DeviceListItem* item = new DeviceListItem(iteration_zones, has_direct);
        ui->devices->layout()->addWidget(item);
        device_items.push_back(item);

        connect(item, &DeviceListItem::SelectionChanged, [=](){
            emit SelectionChanged();
        });
    }

    ((QVBoxLayout*) ui->devices->layout())->addStretch(10000);
}

void EffectsDeviceList::on_toggle_select_all_clicked()
{
    for(DeviceListItem* item: device_items)
    {
        if(item->HasDirect())
        {
            item->SetEnabled(ui->toggle_select_all->isChecked());
        }
    }

    emit SelectionChanged();
}

void EffectsDeviceList::on_toggle_reverse_clicked()
{
    for(DeviceListItem* item: device_items)
    {
        item->SetReverse(ui->toggle_reverse->isChecked());
    }

    emit SelectionChanged();
}

void EffectsDeviceList::on_toggle_brightness_clicked()
{
    for(DeviceListItem* item: device_items)
    {
        item->ToggleBrightnessSlider();
    }
}

void EffectsDeviceList::DisableControls()
{
    setEnabled(false);

    for(DeviceListItem* item: device_items)
    {
        item->DisableControls();
    }
}

void EffectsDeviceList::EnableControls()
{
   setEnabled(true);

    for(DeviceListItem* item: device_items)
    {
        item->EnableControls();
    }
}

std::vector<ControllerZone*> EffectsDeviceList::GetControllerZones()
{
    return controller_zones;
}

std::vector<ControllerZone*> EffectsDeviceList::GetSelection()
{
    std::vector<ControllerZone*> selection;

    for(DeviceListItem* item: device_items)
    {
        for(ControllerZone* controller_zone: item->GetSelection())
        {
            selection.push_back(controller_zone);
        }
    }

    return selection;
}

void EffectsDeviceList::ApplySelection(std::vector<ControllerZone*> selection)
{
    for(DeviceListItem* item: device_items)
    {
        item->ApplySelection(selection);
    }
}

void EffectsDeviceList::ApplyIconFonts()
{
    const QFont icon_font = OpenRGBPluginsFont::GetFont();
    ui->toggle_brightness->setFont(icon_font);
    ui->toggle_brightness->setText(OpenRGBPluginsFont::icon(OpenRGBPluginsFont::sun));
}
