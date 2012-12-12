/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
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

#include "datasource_favorites.h"
#include <KDE/KFilePlacesModel>

#include <QtCore/QTimerEvent>


namespace
{
static const int _UPDATE_DELAY = 1000; // msec
static const int _MAX_ITEMS_NUMBER = 14; // maximum number of items
}


DataSource_Favorites::DataSource_Favorites(QObject *parent)
    : DataSource(parent)
    , m_placesModel(new KFilePlacesModel(this))
    , m_isPlacesChanged(false)
{
    reloadItems();

    connect(m_placesModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(modelReset()), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(placesChanged()));
}

int DataSource_Favorites::getItemCount()
{
    return favoritesList.count();
}

QString DataSource_Favorites::itemUrlDnd(int id)
{
    if (id >= 0 && id < favoritesList.count())
        return favoritesList[id]["desktopEntry"].toString();
    return QString();
}


QVariantMap DataSource_Favorites::getContent(int index)
{
    return favoritesList[index];
}

void DataSource_Favorites::itemClicked(int newIndex)
{
    if (newIndex != -1)
        emit runDesktopFile(favoritesList[newIndex]["desktopEntry"].toString());
}

void DataSource_Favorites::placesChanged()
{
    // simple change of one place cause many events about change => so start timer to skip them
    m_isPlacesChanged = true;
    m_timer.start(_UPDATE_DELAY, this);
}

void DataSource_Favorites::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timer.timerId()) {
        m_timer.stop();
        // OK, numerous events are skipped => update if it's allowed otherwise delay update
        if (isUpdateAllowed())
            reloadItems();
        return;
    }
    DataSource::timerEvent(event);
}

void DataSource_Favorites::onUpdateAllowedChanged()
{
    // if the places are changed the we should update them
    if (m_isPlacesChanged && isUpdateAllowed())
        reloadItems();
}

void DataSource_Favorites::reloadItems()
{
    m_isPlacesChanged = false;
    QList<AppItem> new_list;
    for (int i = 0; i < m_placesModel->rowCount() && new_list.size() < _MAX_ITEMS_NUMBER; i++) {
        QModelIndex index = m_placesModel->index(i, 0);
        KBookmark bm = m_placesModel->bookmarkForIndex(index);
        if (!bm.isNull() && !m_placesModel->isHidden(index)) {
            AppItem newItem;
            newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(bm.icon());
            newItem["caption"] = bm.fullText();
            newItem["id"] = new_list.size();
            newItem["desktopEntry"] = index.data(KFilePlacesModel::UrlRole);
            newItem["description"] = newItem["desktopEntry"].toUrl().toLocalFile();

            new_list.append(newItem);
        }
    }

    favoritesList = new_list;
    emit resetContent();
}
