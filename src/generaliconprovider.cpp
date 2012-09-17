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
#include "datasource_search.h"
#include "swapp.h"

#include <KDebug>
#include <KIcon>

#include <QPainter>

GeneralIconProvider::GeneralIconProvider(QString path_to_assets, QMLConstants *inConstants)
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
      m_userInfoProvider(NULL),
      m_searchGridModel(NULL),
      constants(inConstants),
      m_pathToAssets(path_to_assets)
{

}

QPixmap GeneralIconProvider::requestPixmap(const QString &name, QSize *size, const QSize &requestedSize)
{
//#define icons_size 128
    KIcon icon;
    QPixmap iconPixmap;

    QString iconType = name.section('/', 0, 0, QString::SectionSkipEmpty);
    QString iconName = name.section('/', 1, -1, QString::SectionSkipEmpty);
    //qDebug() << "ICON SIZE:" << constants->iconSize() << " for " << iconName;

    if (iconType == "stacked")
    {
        const int outlineWidth = constants->iconSize() / 19;

        //qDebug() << iconName;
        QStringList icons = iconName.split("|");

        QPixmap pix(constants->iconSize(), constants->iconSize());
        pix.fill(Qt::transparent);

        QPainter p;
        p.begin(&pix);
        p.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

        p.setPen(QPen(Qt::gray, outlineWidth));
        p.setBrush(QBrush(QColor(64, 64, 64, 128)));
        p.drawRoundedRect(outlineWidth, outlineWidth, pix.width() - outlineWidth*2, pix.height() - outlineWidth*2, outlineWidth*2, outlineWidth*2);
        QSize subIconSize = (pix.size() - QSize(outlineWidth*4, outlineWidth*4)) / 2;
        for (int i = 0; i < qMin(4, icons.size()); i++)
        {
            KIcon subIcon = KIcon(icons[i]);
            QPixmap subPixmap = subIcon.pixmap(subIconSize);

            int x = i & 1 ? (pix.width() / 2 - subIconSize.width()) : pix.width()/2;
            int y = i / 2 ? (pix.height() / 2 - subIconSize.height()) : pix.height()/2;
            p.drawPixmap(x, y, subPixmap);
        }
        p.end();


        icon = KIcon(pix);
    }
    else if(iconType == "appicon")
        icon = KIcon(iconName);
    else if(iconType == "search") {
        if(m_searchGridModel)
            icon = KIcon(m_searchGridModel->getMatchIcon(iconName));
    } else if(iconType == "asset") {
        iconPixmap.load(m_pathToAssets + iconName);

        // TODO: Fill size struct

        return iconPixmap;
    } else if(iconType == "general") {
        if(iconName == "usericon" && m_userInfoProvider != NULL)
            icon = KIcon(m_userInfoProvider->getIconPath());
        else
            return iconPixmap;
    } else {
        return iconPixmap;
    }

    size->setWidth(constants->iconSize());
    size->setHeight(constants->iconSize());

    if (requestedSize.isEmpty())
        iconPixmap = icon.pixmap(constants->iconSize(), constants->iconSize(), QIcon::Normal, QIcon::On);
    else
        iconPixmap = icon.pixmap(requestedSize.width(), requestedSize.height(), QIcon::Normal, QIcon::On);

    return iconPixmap;
}
