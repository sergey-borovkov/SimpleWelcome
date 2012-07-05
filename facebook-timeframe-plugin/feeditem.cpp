#include "feeditem.h"

#include <QtCore/QDebug>
#include <qjson/parser.h>

FeedItem::~FeedItem()
{
}

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
    if(map.contains("message"))
        m_data.insert(Text, map.value("message").toString());
    if(map.contains("picture"))
    {
        m_data.insert(Image, map.value("picture").toString());
        qDebug() << map.value("picture").toString();
    }
}
