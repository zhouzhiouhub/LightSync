/*---------------------------------------------------------*\
| TabLabel.cpp                                              |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include "TabLabel.h"
#include "OpenRGBFont.h"
#include "ui_TabLabel.h"

TabLabel::TabLabel(int icon,
                   const QString &name,
                   const QString &sourceText,
                   const char *context) :
    QWidget(nullptr),
    ui(new Ui::TabLabel),
    baseText(sourceText),
    baseTextUtf8(sourceText.toUtf8()),
    translationContext(context ? QByteArray(context) : QByteArray())
{
    ui->setupUi(this);

    QFont font = OpenRGBFont::GetFont();
    font.setPointSize(18);

    ui->icon->setFont(font);
    ui->icon->setText(OpenRGBFont::icon(icon));

    ui->name->setText(name);
}

TabLabel::~TabLabel()
{
    delete ui;
}

void TabLabel::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        /*-----------------------------------------------------*\
        | Preserve the source string and translation context to |
        |   allow language switching without dangling pointers. |
        \*-----------------------------------------------------*/
        if(translationContext.isEmpty())
        {
            ui->name->setText(baseText);
            return;
        }

        const QString translated =
            QCoreApplication::translate(translationContext.constData(), baseTextUtf8.constData());

        ui->name->setText(translated.isEmpty() ? baseText : translated);
    }
}
