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

#ifndef SOCIALITEM_H
#define SOCIALITEM_H

#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QDateTime>

#include <listitem.h>

class CommentItem;
class ListModel;

class TIMEFRAMELIB_EXPORT SocialItem : public ListItem
{
public:
    enum Role {
        Text = Qt::UserRole + 1,
        ImageUrl,
        LikeUrl,
        DateTime,
        Likes,
        Like,
        Comments,
        CommentCount,
        Audio,
        AudioId,
        AudioUrl,
        AudioOwnerId,
        Video,
        VideoId,
        VideoUrl,
        VideoOwnerId,
        VideoImage,
        PluginName,
        Count,
        ItemsCount,
        FromId,
        FromName,
        FromImageUrl,
        Id
    };

    enum Type {
        Post,
        Image
    };

    enum LikeType {
        NotLiked,
        Liked,
        LikeNotAvailable
    };

    SocialItem(const QString & selfId);
    virtual ~SocialItem() {}

    Type type() const;

    virtual QString id() const;
    void setId(const QString & id);

    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant & value);

    ListModel* commentsModel();

    QDateTime datetime() const;

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(Id, "id");
        roles.insert(FromId, "fromId");
        roles.insert(FromName, "fromName");
        roles.insert(FromImageUrl, "fromImageUrl");
        roles.insert(Count, "count");
        roles.insert(PluginName, "pluginName");
        roles.insert(Text, "message");
        roles.insert(ImageUrl, "picture");
        roles.insert(Audio, "audio");
        roles.insert(AudioId, "audioId");
        roles.insert(AudioUrl, "audioUrl");
        roles.insert(AudioOwnerId, "audioOwnerId");
        roles.insert(Video, "video");
        roles.insert(VideoId, "videoId");
        roles.insert(VideoUrl, "videoUrl");
        roles.insert(VideoOwnerId, "videoOwnerId");
        roles.insert(VideoImage, "videoImage");
        roles.insert(Like, "like");
        roles.insert(Likes, "likes");
        roles.insert(LikeUrl, "likeUrl");
        roles.insert(CommentCount, "commentCount");
        roles.insert(Comments, "comments");
        roles.insert(Count, "count");
        roles.insert(ItemsCount, "size");
        roles.insert(DateTime, "datetime");

        return roles;
    }

private:
    QString m_id;
    QMap<int, QVariant> m_data;
    ListModel *m_commentsModel;
    QString m_selfId;

};

#endif // SOCIALITEM_H
