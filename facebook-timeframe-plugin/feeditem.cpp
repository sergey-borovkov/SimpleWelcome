#include "feeditem.h"
#include <qjson/parser.h>

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

void FeedItem::fillFromMap(QVariantMap map)
{
    // http://developers.facebook.com/docs/reference/api/post/
    QString id = map.value("id").toString();
    QString from = map.value("from").toString();
    QString to = map.value("to").toString();
    QString message = map.value("message").toString();
    QString picture = map.value("picture").toString();
    QString link = map.value("link").toString();
    QString icon = map.value("name").toString();

    QString caption = map.value("caption").toString();
    QString description = map.value("description").toString();

    if(map.contains("id"))
        m_id = map.value("id").toString();

//    if(map.contains(""))
}
