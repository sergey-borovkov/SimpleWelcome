#ifndef SOCIALITEM_H
#define SOCIALITEM_H

#include <QtCore/QHash>

class SocialItem
{
public:
    virtual ~SocialItem() = 0;
    virtual QString pluginName() const = 0;
    virtual QString type() const = 0;
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
};

#endif // SOCIALITEM_H
