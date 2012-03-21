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

#include "sessionprovider.h"

SessionProvider::SessionProvider(void) : QObject()
{
}

SessionProvider::~SessionProvider(void)
{
}

void SessionProvider::init(void)
{
  
}

void SessionProvider::lock(void)
{
  QDBusMessage message = QDBusMessage::createMethodCall("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", "Lock");

  QDBusConnection::sessionBus().send(message);
}

void SessionProvider::shutdown(void)
{
  QDBusMessage message = QDBusMessage::createMethodCall("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", "logout");

  message << -1 << -1 << -1; // Arguments

  QDBusConnection::sessionBus().send(message);
}

#include "sessionprovider.moc"
