#ifndef SOCIALITEM_H
#define SOCIALITEM_H

#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QVariant>

#include <listitem.h>


class SocialItem : public ListItem
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
        PluginName,
        Count,
        ItemsCount,
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

    virtual ~SocialItem() {}
    virtual QString pluginName() const = 0;
    virtual Type type() const = 0;
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QDateTime datetime() const = 0;

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(Id, "id");
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
};

#endif // SOCIALITEM_H
