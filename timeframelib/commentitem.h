/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include <QtCore/QMetaType>
#include <QtCore/QHash>
#include <QtCore/QMap>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>

#include "listitem.h"

class TIMEFRAMELIB_EXPORT CommentItem : public ListItem
{
public:
    enum Role {
        From = Qt::UserRole + 1,
        FromId,
        FromPictureUrl,
        Message,
        CreatedTime,
        LikeCount,
        Id,
        Type
    };
    ~CommentItem();

    virtual QString id() const;

    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(From, "from");
        roles.insert(FromId, "fromId");
        roles.insert(FromPictureUrl, "fromPictureUrl");
        roles.insert(Message, "messageText");
        roles.insert(CreatedTime, "createdTime");
        roles.insert(LikeCount, "likeCount");
        roles.insert(Id, "id");
        roles.insert(Type, "pluginName");
        return roles;
    }

private:
    QMap<int, QVariant> m_data;
};

Q_DECLARE_METATYPE(CommentItem *)
Q_DECLARE_METATYPE(QList<CommentItem *>)

#endif
