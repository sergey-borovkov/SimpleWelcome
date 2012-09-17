/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
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

#include "rosa-starter.h"

#include <QtDBus/QtDBus>

#include <QPainter>
#include <QGraphicsWidget>
#include <QIcon>
#include <QObject>
#include <QTimer>
#include <QProcess>

#include <Plasma/IconWidget>

#include <KRun>

#include <klocalizedstring.h>

#include "config.h"

void ROSA_Starter::init()
{
    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::Square);

    m_layout = new QGraphicsLinearLayout(Qt::Vertical);
    m_layout->setContentsMargins(0, 4, 0, 5);
    setLayout(m_layout);

    setMinimumWidth(10);
    setMinimumHeight(10);

    m_iconSize = 48;

    if(formFactor() == Plasma::Vertical)
        m_iconSize = topLevelWidget()->size().width();
    else if(formFactor() == Plasma::Horizontal)
        m_iconSize = topLevelWidget()->size().height();

    resize(m_iconSize, m_iconSize);

    m_icon = new Plasma::IconWidget();
    m_icon->setIcon(QIcon(QString(PIXMAP_PATH) + "rosa-icon.png"));
    m_layout->addItem(m_icon);

    connect(this, SIGNAL(geometryChanged()), SLOT(resetIconSize()));
    connect(m_icon, SIGNAL(clicked()), SLOT(buttonClicked()));

    KRun::runCommand("rosa-launcher -s", NULL);
}

void ROSA_Starter::resetIconSize()
{
    m_icon->update(m_icon->boundingRect());
}

void ROSA_Starter::buttonClicked()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    QDBusInterface interface("org.kde.ROSA_Launcher", "/MainApplication", "org.kde.KUniqueApplication", sessionBus, this);

    if(interface.isValid()) // if SW is launched
        interface.call("newInstance");
    else
        QProcess::execute("rosa-launcher");
}

K_EXPORT_PLASMA_APPLET(rosastarter, ROSA_Starter)
