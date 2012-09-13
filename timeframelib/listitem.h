#ifndef LISTITEM_H
#define LISTITEM_H

#include "timeframelib_global.h"

class TIMEFRAMELIB_EXPORT ListItem
{
public:
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual bool setData(const QVariant & value, int role) = 0;
};

#endif // LISTITEM_H
