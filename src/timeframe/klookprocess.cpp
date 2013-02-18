/* Timeframe
 * Copyright (c) 2013 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 * License: GPLv3
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

#include "klookprocess.h"

#include <KProcess>

#include <QDBusInterface>
#include <QDBusConnectionInterface>
#include <QRect>

KlookProcess::KlookProcess(QObject *parent) :
    QObject(parent), m_processStarted(false)
{
}

void KlookProcess::show(int x, int y, int width, int height, const QString &path)
{
    show(QRect(x, y, width, height), path);
}

void KlookProcess::show(const QRect &rectangle, const QString &path)
{
    QDBusInterface interface("org.kde.klook", "/Application", "org.kde.klook.Application",
                             QDBusConnection::sessionBus(), this);
    if (interface.isValid())  { // if Klook is launched
        interface.call("showEmbedded", rectangle, path);
    }
    else {
        KProcess klook;
        klook << QLatin1String("klook") << QLatin1String("--donotquit") << QLatin1String("--embedded")
              << QLatin1String("-x") << QString::number(rectangle.x()) << QLatin1String("-y") << QString::number(rectangle.y())
              << QLatin1String("-w") << QString::number(rectangle.width()) << QLatin1String("-h")
              << QString::number(rectangle.height()) << path;
        klook.startDetached();
    }

    m_processStarted = true;
}

void KlookProcess::hide()
{
    if(!m_processStarted)
        return;

    QDBusInterface interface("org.kde.klook", "/Application", "org.kde.klook.Application",
                             QDBusConnection::sessionBus(), this);
    if (interface.isValid())
        interface.call("hide");

    m_processStarted = false;
}
