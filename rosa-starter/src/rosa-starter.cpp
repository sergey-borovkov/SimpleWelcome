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

// Own
#include "config.h"
#include "rosa-starter.h"

// Qt
#include <QtDBus/QtDBus>
#include <QtGui/QAction>
#include <QtGui/QGraphicsLinearLayout>

// KDE
#include <KDE/Plasma/IconWidget>
#include <KDE/KRun>
#include <KDE/KGlobal>
#include <KDE/KLocale>


QString ROSA_Starter::pathToRoot()
{
    QDir root_dir = QCoreApplication::applicationDirPath();
    root_dir.cdUp(); // skip 'bin' directory
    return root_dir.canonicalPath();
}

void ROSA_Starter::init()
{
    // We must add translations from SimpleWelcome
    KGlobal::locale()->insertCatalog(SW_APPLICATION_NAME);

    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::Square);

    m_layout = new QGraphicsLinearLayout(Qt::Vertical);
    m_layout->setContentsMargins(0, 4, 0, 5);
    setLayout(m_layout);

    setMinimumWidth(10);
    setMinimumHeight(10);

    m_iconSize = 48;

    if (formFactor() == Plasma::Vertical)
        m_iconSize = topLevelWidget()->size().width();
    else if (formFactor() == Plasma::Horizontal)
        m_iconSize = topLevelWidget()->size().height();

    resize(m_iconSize, m_iconSize);

    m_icon = new Plasma::IconWidget();
    m_icon->setIcon(QIcon(pathToRoot() + QString::fromLatin1("/" SW_ASSETS_PATH "/") + "rosa-icon.png"));
    m_layout->addItem(m_icon);

    connect(this, SIGNAL(geometryChanged()), SLOT(resetIconSize()));
    connect(m_icon, SIGNAL(clicked()), SLOT(buttonClicked()));

    QAction *menuEditor = new QAction(i18n("Edit Applications..."), this);
    m_actions.append(menuEditor);
    connect(menuEditor, SIGNAL(triggered(bool)), SLOT(startMenuEditor()));


    KRun::runCommand("rosa-launcher -s", NULL);
}

QList<QAction *> ROSA_Starter::contextualActions()
{
    return m_actions;
}

void ROSA_Starter::resetIconSize()
{
    m_icon->update(m_icon->boundingRect());
}

void ROSA_Starter::buttonClicked()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    QDBusInterface interface("org.kde." SW_APPLICATION_NAME, "/MainApplication", "org.kde.KUniqueApplication", sessionBus, this);

    if (interface.isValid()) // if SW is launched
        interface.call("newInstance");
    else
        KRun::runCommand("rosa-launcher", NULL);
}

void ROSA_Starter::startMenuEditor()
{
    KRun::runCommand("kmenuedit", NULL);
}

K_EXPORT_PLASMA_APPLET(rosastarter, ROSA_Starter)
