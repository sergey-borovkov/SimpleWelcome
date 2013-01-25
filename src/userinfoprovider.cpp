/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 * Alexander Skakov <alexander.skakov@osinit.ru>
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

#include "userinfoprovider.h"
#include <QProcessEnvironment>

QString UserInfoProvider::getFullName()
{
    return m_userInfo.property(KUser::FullName).toString();
}

QString UserInfoProvider::getLoginName()
{
    return m_userInfo.loginName();
}

QString UserInfoProvider::getUserName()
{
    QString username = getFullName();

    if (username.isEmpty())
        username = m_userInfo.loginName();

    return username;
}

QPixmap UserInfoProvider::getFaceIcon()
{
    QPixmap res;

    // First trying to get user icon from KDE
    QString facePath = m_userInfo.faceIconPath();
    res = QPixmap(facePath);

    if (res.isNull()) {
        // If it fails, trying to get environment variable $XDG_DATA_DIRS
        QString prefix;
        const char *envVarStr = "XDG_DATA_DIRS="; // XDG_DATA_DIRS defaults to /usr/share in Rosa
        QStringList envs = QProcessEnvironment::systemEnvironment().toStringList();
        foreach(QString env, envs) {
            if (env.startsWith(envVarStr)) {
                QStringList xdgDirs = env.right(env.size() - strlen(envVarStr)).split(":");
                if (xdgDirs.size())
                    prefix = xdgDirs[0] + "/faces";
            }
        }

        // Trying $XDG_DATA_DIRS/faces/$USER_LOGIN_NAME.png
        facePath = QString("%1/%2.png").arg(prefix, m_userInfo.loginName());
        res = QPixmap(facePath);

        if (res.isNull()) {
            // If this fails, trying $XDG_DATA_DIRS/faces/default.png
            facePath = QString("%1/default.png").arg(prefix);
            res = QPixmap(facePath);

            // If even THIS fails, using default placeholder icon in generaliconprovider
        }
    }
    return res;
}
