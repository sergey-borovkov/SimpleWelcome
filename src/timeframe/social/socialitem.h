#ifndef SOCIALITEM_H
#define SOCIALITEM_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "../listitem.h"

class SocialItem : public ListItem
{
public:
    enum Role
    {
        Text,
        ImageUrl,
        LikeUrl,
        Date,
        Likes,
        Comments,
        Audio,
        PluginName
    };

    enum Type
    {
        Post,
        Image
    };

    virtual ~SocialItem() {}
    virtual QString pluginName() const = 0;
    virtual Type type() const = 0;
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;

    static const QHash<int,QByteArray> roleNames()
    {
        QHash<int,QByteArray> roles;
        roles.insert(Text, "message");
        roles.insert(Image, "picture");
        roles.insert(Date, "date");
        roles.insert(Likes, "likes");
        roles.insert(Comments, "comments");
        roles.insert(Audio, "audio");
        roles.insert(PluginName, "pluginName");

        return roles;
    }
};

#endif // SOCIALITEM_H
