#include "feeditem.h"

#include <QtCore/QDebug>
#include <qjson/parser.h>

QString FeedItem::pluginName() const
{
    return QLatin1String("facebook");
}

FeedItem::Type FeedItem::type() const
{
    return Post;
}

QString FeedItem::id() const
{
    return m_id;
}

QVariant FeedItem::data(int role) const
{
    return m_data.value(role);
}

void FeedItem::fillFromMap(QVariantMap map)
{
    // http://developers.facebook.com/docs/reference/api/post/
    if(map.contains("id"))
        m_id = map.value("id").toString();
    if(map.contains("message"))
        m_data.insert(Text, map.value("message").toString());
    if(map.contains("picture"))
    {
        m_data.insert(Image, map.value("picture").toString());
        qDebug() << map.value("picture").toString();
    }
}
