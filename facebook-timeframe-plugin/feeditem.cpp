#include "feeditem.h"

QString FeedItem::pluginName() const
{
    return QLatin1String("facebook");
}

QString FeedItem::type() const
{
    return QLatin1String("post");
}

QString FeedItem::id() const
{
    return m_id;
}

QVariant FeedItem::data(int role) const
{
    return QVariant();
}
