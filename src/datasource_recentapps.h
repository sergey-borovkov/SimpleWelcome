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

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_RecentApps(QObject *parent = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);
    void addRecentApp(QString desktopFile, bool isPinned = false, bool isSimplyLoad = false);

signals:
    void newItemData(QVariantMap itemData, QString group) const;
    void runDesktopFile(QString desktopFile);
    void updateItemData(int id, QString field, QVariant data);

public slots:
    virtual void itemClicked(int newIndex);
    void updateContent();
    void itemPinnedToggle(int index);
    void itemDragged(int fromIndex, int toIndex);
    void ksycocaChanged(const QStringList changes);
    bool checkApps();

private:
    void saveData();
    void moveIconSkippingPinned(int id);
    bool m_isDbChanged;

    AppItemList recentAppsList;
};
