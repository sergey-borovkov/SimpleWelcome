/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 * Alexander Skakov <alexander.skakov@osinit.ru>
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

#include <QObject>
#include <KUser>
#include <QPixmap>

class UserInfoProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullName READ getFullName)
    Q_PROPERTY(QString loginName READ getLoginName)
    Q_PROPERTY(QString userName READ getUserName)

public:
    UserInfoProvider(QObject *parent = 0)
        : QObject(parent) {}

    QString getFullName();
    QString getLoginName();
    QString getUserName();
    QPixmap getFaceIcon();

private:
    KUser m_userInfo;
};
