/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
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

#include "appitem.h"
#include "datasource.h"

#include <QtCore/QModelIndex>
#include <QtCore/QBasicTimer>

// Forward declarations
class KFilePlacesModel;

class DataSource_Favorites : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Favorites(QObject *parent = 0);

    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);


private slots:
    void placesChanged();

private:
    virtual void timerEvent(QTimerEvent *event);
    virtual void onUpdateAllowedChanged();

    void reloadItems();

    AppItemList favoritesList;
    QBasicTimer m_timer; // we use timer to skip dataChanged events and don't reload items very often
    KFilePlacesModel *m_placesModel;
    bool m_isPlacesChanged;
};
