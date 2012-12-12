/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#include "commentitem.h"

CommentItem::~CommentItem()
{
}

QString CommentItem::id() const
{
    return data(Id).toString();
}

QVariant CommentItem::data(int role) const
{
    if (role == CommentItem::FromPictureUrl) {
        if (m_data.value(role).toString().isEmpty())
            return QVariant("images/user.png");
    }
    return m_data.value(role);
}

bool CommentItem::setData(int role, const QVariant &value)
{
    m_data.insert(role, value);
    return true;
}
