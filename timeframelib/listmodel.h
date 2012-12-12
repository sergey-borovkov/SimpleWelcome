/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
 * License: GPLv3
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

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QMetaType>

#include "timeframelib_global.h"

class ListItem;

#pragma GCC diagnostic ignored "-Woverloaded-virtual"

class TIMEFRAMELIB_EXPORT ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    ~ListModel();
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual void appendRow(ListItem *item);
    virtual void appendRows(const QList<ListItem *> &items);
    virtual void prependRow(ListItem *item);
    virtual void prependRows(const QList<ListItem *> &items);
    virtual void insertRow(int row, ListItem *item);
    virtual bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void refreshRow(int row);
    ListItem *takeRow(int row);
    ListItem *find(const QString &id) const;
    ListItem *itemAt(int row) const;
    QModelIndex indexFromItem(const ListItem *item) const;
    void clear();
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<ListItem *> m_items;
    QHash<int, QByteArray> m_roles;
};

Q_DECLARE_METATYPE(ListModel *)

#endif // LISTMODEL_H


