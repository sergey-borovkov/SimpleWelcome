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

#include "generaliconprovider.h"
#include "userinfoprovider.h"

#include <KDebug>
#include <KIcon>

GeneralIconProvider::GeneralIconProvider()
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
      m_isLocal(true),
      m_userInfoProvider(0)
{

}

QPixmap GeneralIconProvider::requestPixmap(const QString &name, QSize *size, const QSize &requestedSize)
{
    KIcon icon;
    QPixmap iconPixmap;

    QString iconType = name.section('/', 0, 0, QString::SectionSkipEmpty);
    QString iconName = name.section('/', 1, -1, QString::SectionSkipEmpty);

    if(iconType == "appicon")
        icon = KIcon(iconName);
    else if(iconType == "asset")
    {
        if(m_isLocal)
            iconPixmap.load(QString("../assets/") + iconName);
        else
            iconPixmap.load(QString("/usr/share/rosa-launcher-qtquick/assets/") + iconName);

      // TODO: Fill size struct

        return iconPixmap;
    }
    else if(iconType == "general")
    {
        if(iconName == "usericon" && m_userInfoProvider != NULL)
            icon = KIcon(m_userInfoProvider->getIconPath());
        else
            return iconPixmap;
    }
    else
    {
        return iconPixmap;
    }

    size->setWidth(128);
    size->setHeight(128);

    if (requestedSize.isEmpty())
        iconPixmap = icon.pixmap(128, 128, QIcon::Normal, QIcon::On);
    else
        iconPixmap = icon.pixmap(requestedSize.width(), requestedSize.height(), QIcon::Normal, QIcon::On);

    return iconPixmap;
}
