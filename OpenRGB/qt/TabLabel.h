/*---------------------------------------------------------*\
| TabLabel.h                                                |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

namespace Ui
{
    class TabLabel;
}

class TabLabel : public QWidget
{
    Q_OBJECT

public:
    TabLabel(int icon,
             const QString &displayName,
             const QString &sourceText,
             const char *context);
    ~TabLabel();

private:
    Ui::TabLabel *ui;
    QString       baseText;
    QByteArray    baseTextUtf8;
    QByteArray    translationContext;

private slots:
    void changeEvent(QEvent *event);
};
