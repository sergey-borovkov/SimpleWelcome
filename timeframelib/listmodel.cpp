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

#include "listmodel.h"
#include "listitem.h"

ListModel::ListModel(QHash<int, QByteArray> roles, QObject *parent) :
    QAbstractListModel(parent), m_roles(roles)
{
    setRoleNames(m_roles);
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.size())
        return QVariant();
    return m_items.at(index.row())->data(role);
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    ListItem *item = m_items.at(index.row());
    bool result = item->setData(role, value);
    if (result) {
        emit dataChanged(index, index);
    }
    return result;
}

ListModel::~ListModel()
{
    clear();
}

void ListModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem *>() << item);
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach(ListItem * item, items)
    m_items.append(item);
    endInsertRows();
}


void ListModel::prependRow(ListItem *item)
{
    prependRows(QList<ListItem *>() << item);
}

void ListModel::prependRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), 0, items.size() - 1);
    foreach(ListItem * item, items)
        m_items.prepend(item);
    endInsertRows();
}

void ListModel::insertRow(int row, ListItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

ListItem *ListModel::find(const QString &id) const
{
    foreach(ListItem * item, m_items)
    if (item->id() == id)
        return item;
    return 0;
}

ListItem *ListModel::itemAt(int row) const
{
    return m_items[row];
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    Q_ASSERT(item);
    for (int row = 0; row < m_items.size(); ++row)
        if (m_items.at(row) == item)
            return index(row);

    return QModelIndex();
}

void ListModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || row >= m_items.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_items.takeAt(row);
    endRemoveRows();
    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || (row + count) > m_items.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        delete m_items.takeAt(row);
    endRemoveRows();
    return true;
}

void ListModel::refreshRow(int row)
{
    QModelIndex ind = index(row);
    emit dataChanged(ind, ind);
}

ListItem *ListModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ListItem *item = m_items.takeAt(row);
    endRemoveRows();
    return item;
}
