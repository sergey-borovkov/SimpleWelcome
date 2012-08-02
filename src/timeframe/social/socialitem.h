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
        Text = Qt::UserRole + 1,
        ImageUrl,
        LikeUrl,
        Date,
        Likes,
        Comments,
        Audio,
        PluginName,
        Count,
        ItemsCount
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
    virtual QVariant data( int role ) const = 0;
    virtual QDate date() const = 0;

    static const QHash<int, QByteArray> roleNames()
    {
        QHash<int, QByteArray> roles;
        roles.insert( Count,        "count" );
        roles.insert( PluginName,   "pluginName" );
        roles.insert( Text,         "message" );
        roles.insert( ImageUrl,     "picture" );
        roles.insert( Audio,        "audio" );
        roles.insert( Likes,        "likes" );
        roles.insert( Comments,     "comments" );
        roles.insert( Count,        "count" );
        roles.insert( ItemsCount,   "size" );
        roles.insert( Date,   "date" );

        return roles;
    }
};

#endif // SOCIALITEM_H
