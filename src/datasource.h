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

#include <QtCore/QObject>

class DataSource : public QObject
{
    Q_OBJECT
public:
    DataSource(QObject *parent = 0);
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)
    Q_PROPERTY(bool isUpdateAllowed READ isUpdateAllowed WRITE setUpdateAllowed NOTIFY updateAllowedChanged)

    QString getQmlGroupName() {return qmlGroupName;}
    void setQmlGroupName(QString inQmlGroupName) {qmlGroupName = inQmlGroupName;}

    void setUpdateAllowed(bool allow);
    bool isUpdateAllowed() const { return m_isUpdateAllowed; }

signals:
    void resetContent();
    void updateItemData(int id, QString field, QString data);
    void runDesktopFile(QString desktopFile);
    void updateAllowedChanged();

protected:
    QString qmlGroupName;

private:
    virtual void onUpdateAllowedChanged() {}

    bool m_isUpdateAllowed; // to disallow updates if window is shown
};
