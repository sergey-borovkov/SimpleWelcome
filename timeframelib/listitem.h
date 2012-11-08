#ifndef LISTITEM_H
#define LISTITEM_H

#include "timeframelib_global.h"

class QVariant;

class TIMEFRAMELIB_EXPORT ListItem
{
public:
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual bool setData(int role, const QVariant &value) = 0;
};

#endif // LISTITEM_H
