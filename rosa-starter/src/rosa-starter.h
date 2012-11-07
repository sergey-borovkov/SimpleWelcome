/*
 * Copyright (c) ROSA Laboratory 2011-2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#include <Plasma/Applet>

class QGraphicsLinearLayout;
namespace Plasma {
class IconWidget;
}

class ROSA_Starter : public Plasma::Applet
{
    Q_OBJECT

public:
    ROSA_Starter(QObject *parent, const QVariantList &args)
        : Plasma::Applet(parent, args) {}
    virtual void init();
    virtual QList<QAction*> contextualActions();

private slots:
    void resetIconSize();
    void buttonClicked();
    void startMenuEditor();

private:
    QString pathToRoot();

    Plasma::IconWidget *m_icon;
    QGraphicsLinearLayout *m_layout;
    int m_iconSize;

    QList<QAction*> m_actions;
};
