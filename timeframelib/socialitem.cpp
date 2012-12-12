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

#include "socialitem.h"

#include <commentitem.h>
#include <listmodel.h>

SocialItem::SocialItem(const QString &selfId)
    : m_selfId(selfId)
{
    m_commentsModel = new ListModel(CommentItem::roleNames());

    QVariant var;
    var.setValue(m_commentsModel);
    setData(Comments, var);
}

SocialItem::Type SocialItem::type() const
{
    return Post;
}

QString SocialItem::id() const
{
    return m_id;
}

void SocialItem::setId(const QString &id)
{
    m_id = id;
}

QVariant SocialItem::data(int role) const
{
    if (role == Id)
        return m_id;
    else
        return m_data.value(role);
}

bool SocialItem::setData(int role, const QVariant &value)
{
    m_data[role] = value;
    return true;
}

ListModel *SocialItem::commentsModel()
{
    return m_commentsModel;
}

QDateTime SocialItem::datetime() const
{
    return data(DateTime).toDateTime();
}
