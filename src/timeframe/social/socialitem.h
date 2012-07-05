#ifndef SOCIALITEM_H
#define SOCIALITEM_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

class SocialItem
{
public:
    enum Role
    {
        Text,
        ImageUrl,
        LikeUrl
    };

    enum Type
    {
        Post,
        Image
    };

    virtual ~SocialItem() = 0;
    virtual QString pluginName() const = 0;
    virtual Type type() const = 0;
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
};

#endif // SOCIALITEM_H
