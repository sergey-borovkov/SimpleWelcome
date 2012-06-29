#ifndef FEEDITEM_H
#define FEEDITEM_H

#include "../src/timeframe/social/socialitem.h"
#include <QtCore/QMap>

class FeedItem : public SocialItem
{
public:
    virtual QString pluginName() const;
    virtual QString type() const;
    virtual QString id() const;
    virtual QVariant data(int role) const;
private:
    QString m_id;
    QMap<int, QString> m_data;
};

#endif // FEEDITEM_H
