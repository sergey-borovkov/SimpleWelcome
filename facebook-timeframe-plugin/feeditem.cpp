#include "feeditem.h"

#include <QtCore/QDebug>
#include <qjson/parser.h>

QString FeedItem::pluginName() const
{
    return QLatin1String("Facebook");
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
    {
        QString message = map.value("message").toString();

        // if user posts a link
        bool isLink = message.startsWith("http://");
        if(isLink)
        {
            QString name = isLink ? map.value("name").toString() : message;
            m_data.insert(Text, QString("<a href=%1>%2</a>").arg(message, name));
        }
        else
            m_data.insert(Text, message);
    }
    if(map.contains("picture"))
        m_data.insert(ImageUrl, map.value("picture").toString());

    if(map.contains("story"))
    {
        m_data.insert(Text, map.value("story").toString());
    }

    m_data.insert(PluginName, pluginName());
}
