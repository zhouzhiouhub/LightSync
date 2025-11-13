#include "EffectsColorPicker.h"
#include "ui_EffectsColorPicker.h"

#include <QString>
#include <QFile>
#include <QDialog>
#include <QVBoxLayout>
#include <QColorDialog>
#include "ColorUtils.h"

EffectsColorPicker::EffectsColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectsColorPicker)
{
    ui->setupUi(this);
    ui->button->setStyleSheet("QPushButton {background-color: black; border: 1px solid black;}");
}

EffectsColorPicker::~EffectsColorPicker()
{
    delete ui;
}

void EffectsColorPicker::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void EffectsColorPicker::SetQColor(QColor color)
{
    current_color = color;
    ui->button->setStyleSheet("QPushButton {background-color: "+ color.name() + "; border: 1px solid black;}");
}

void EffectsColorPicker::SetRGBColor(RGBColor color)
{
    SetQColor(ColorUtils::toQColor(color));
}

void EffectsColorPicker::on_button_clicked()
{
    QColorDialog *colorDialog = new QColorDialog(this);
    colorDialog->setAttribute(Qt::WA_DeleteOnClose);
    colorDialog->setCurrentColor(current_color);

    connect(colorDialog, &QColorDialog::colorSelected,[=](const QColor &color){
               SetQColor(color);
               emit ColorSelected(color);
    });

    colorDialog->open();
}

QColor EffectsColorPicker::CurrentQColor()
{
    return current_color;
}

RGBColor EffectsColorPicker::CurrentRGBColor()
{
    return ColorUtils::fromQColor(current_color);
}
