/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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

#include "activity.h"
#include "localdaymodel.h"
#include "localcontentitem.h"
#include "localcontentmodel.h"

#include <QtCore/QDate>
#include <QtCore/QVariant>

LocalContentItem::LocalContentItem(const QDate &date, QObject *parent) :
    QObject(parent), m_model(0)
{
    m_itemModel = new LocalDayModel(date, this);
    m_model = new LocalDayFilterModel(this);
    m_model->setSourceModel(m_itemModel);
    m_model->setDynamicSortFilter(true);
    m_model->setFilterRole(LocalDayModel::TypeRole);

    LocalContentModel *model = qobject_cast<LocalContentModel *>(parent);
    if (model)
        m_model->setFilterRegExp(model->filter());
}

QString LocalContentItem::id() const
{
    return "";
}

QVariant LocalContentItem::data(int role) const
{
    if (role == CurrentDateRole) {
        return getDate();
    } else if (role == ItemsRole) {
        return 0;
    } else if (role == TypesRole) {
        return types();
    } else if (role == ItemsCountRole) {
        return getCount();
    }
    return QVariant();
}

bool LocalContentItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    return false;
}

void LocalContentItem::setDate(const QDate &d)
{
    m_itemModel->setDate(d);
    emit dataChanged();
}

void LocalContentItem::addActivity(Activity *item)
{
    if (m_itemModel->rowCount(QModelIndex()) > 500)
        return;

    if (!m_types.contains(item->type())) {
        m_types += (';' + item->type());
    }

    m_itemModel->addActivityItem(item);
    emit dataChanged();
}

QDate LocalContentItem::getDate() const
{
    return m_itemModel->date();
}

int LocalContentItem::getCount() const
{
    return m_model->rowCount(QModelIndex());
}

LocalDayFilterModel *LocalContentItem::model()
{
    return m_model;
}

QHash<int, QByteArray> LocalContentItem::roleNames()
{
    QHash<int, QByteArray> hash;
    hash.insert(ItemsRole, "items");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(TypesRole, "type");
    hash.insert(ItemsCountRole, "size");
    return hash;
}

QString LocalContentItem::types() const
{
    return m_types;
}
