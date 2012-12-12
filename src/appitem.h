/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
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

#include <QString>
#include <QDebug>
#include <QtCore/QStringList>
#include <QApplication>
#include <QHash>
#include <QVariantMap>

/* Item in QML is a dictionary with the following fields:
 * -----
 * imagePath - path to image provider to get icon
 * caption - the one that's displayed under icon
 * id - uniqie identifier, used to interact between C++ and QML
 * pinned - reserved for use in Recent Documents pinning
 * group - used in Apps to pass the group app belong to
 * stack - items stacked in this item, array of dictionaries // auto set to undefined
*/

typedef QVariantMap AppItem;
typedef QList<AppItem> AppItemList;

inline uint qHash(const AppItem &item)
{
    return qHash(item["caption"].toString());
}
