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
#include "datasource_documents.h"
#include "sizescalculator.h"

#include <KIcon>

#include <QPainter>
#include <KIO/PreviewJob>

GeneralIconProvider::GeneralIconProvider(QString path_to_assets, SizesCalculator *inConstants)
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
      m_userInfoProvider(NULL),
      m_searchDataSource(NULL),
      constants(inConstants),
      m_pathToAssets(path_to_assets)
{
}

QPixmap GeneralIconProvider::requestPixmap(const QString &name, QSize *size, const QSize &requestedSize)
{
    KIcon icon;
    QPixmap iconPixmap;

    QString iconType = name.section('/', 0, 0);
    QString iconName = name.section('/', 1, -1);
    //qDebug() << "ICON SIZE:" << constants->iconSize() << " for " << iconName;

    if (iconType == "stacked") {
        //qDebug() << iconName;
        QStringList icons = iconName.split("|");

        QPixmap pix(constants->iconSize(), constants->iconSize());
        pix.fill(Qt::transparent);

        QPainter p;
        p.begin(&pix);
        p.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);


        // Outline
        const int outlineWidth = 4;//constants->iconSize() / 19;

        int iconsInRow = constants->iconSize() >= 96 ? 3 : 2;
        int subIconsMargin = 2;
        QSize subIconSize = (pix.size() - QSize(outlineWidth * 4, outlineWidth * 4) - QSize(subIconsMargin, subIconsMargin) * (iconsInRow - 1)) / iconsInRow;
        int displayIconsCount = qMin(iconsInRow * iconsInRow, icons.size());
        for (int i = 0; i < displayIconsCount; i++) {
            KIcon subIcon = KIcon(icons[icons.size() - displayIconsCount + i]);
            QPixmap subPixmap = subIcon.pixmap(subIconSize);

            int row = i % iconsInRow;
            int col = i / iconsInRow;
            int x = outlineWidth * 2 + row * subIconSize.width() + row * subIconsMargin;
            int y = outlineWidth * 2 + col * subIconSize.height() + col * subIconsMargin;
            p.drawPixmap(x, y, subPixmap);
        }

        p.end();


        icon = KIcon(pix);
    }
    else if (iconType == "appicon")
        icon = KIcon(iconName);
    else if (iconType == "docicon") {
        if (m_documentsDataSource) {
            icon = KIcon(m_documentsDataSource->getPreview(iconName));
            size->setWidth(constants->thumbnailsSize());
            size->setHeight(constants->thumbnailsSize());
            iconPixmap = icon.pixmap(*size, QIcon::Normal, QIcon::On);

            return iconPixmap;
        }
    }
    else if (iconType == "search") {
        if (m_searchDataSource)
            icon = KIcon(m_searchDataSource->getMatchIcon(iconName));
    }
    else if (iconType == "asset") {
        iconPixmap.load(m_pathToAssets + iconName);

        if (!requestedSize.isEmpty())
            iconPixmap = iconPixmap.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // TODO: Fill size struct

        return iconPixmap;
    }
    else if (iconType == "general") {
        if (iconName == "usericon" && m_userInfoProvider != NULL)
            icon = KIcon(m_userInfoProvider->getIconPath());
        else
            return iconPixmap;
    }
    else {
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
