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

#include "appiconprovider.h"

#include <KIcon>
#include <KDebug>

AppIconProvider::AppIconProvider()
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{

}

AppIconProvider::~AppIconProvider()
{
    qDebug() << "DESTROY DESTROY DESTROY !!!";
}

QPixmap AppIconProvider::requestPixmap(const QString &name, QSize *size, const QSize &requestedSize)
{
    qDebug() << "REQUEST REQUEST REQUEST !!!";

    KIcon icon(name);

    size->setWidth(128);
    size->setHeight(128);

    QPixmap iconPixmap;

    if (requestedSize.isEmpty())
    {
        iconPixmap = icon.pixmap(128, 128, QIcon::Normal, QIcon::On);
    }
    else
    {
        iconPixmap = icon.pixmap(requestedSize.width(), requestedSize.height(), QIcon::Normal, QIcon::On);
    }

    return iconPixmap;
}
