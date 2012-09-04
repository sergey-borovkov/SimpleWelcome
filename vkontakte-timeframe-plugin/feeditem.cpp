#include "feeditem.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>

#include <qjson/parser.h>

QString FeedItem::pluginName() const
{
    return QLatin1String("VKontakte");
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

QDate FeedItem::date() const
{
    return QDate::fromString(data(Date).toString(), QString("d MM yyyy"));
}

void FeedItem::fillFromMap(QVariantMap map)
{
    // http://vk.com/developers.php?oid=-1&p=%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D0%BE%D0%B2_API
    if(map.contains("id")) {
        m_id = map.value("id").toString();
    }

    if ( map.contains( "text") ) {
        QString message = map.value( "text" ).toString();
        m_data.insert( Text, message );
    }

    if(map.contains("date")) {
        uint t  = map.value("date").toUInt();
        QDateTime dt;
        dt.setTime_t(t);
        QDate date = dt.date();
        m_data.insert(Date, date.toString("d MM yyyy"));
    }

    if(map.contains("attachments")) {
        QVariantList attachmentList = map[ "attachments" ].toList();

        foreach(QVariant item, attachmentList) {
            QVariantMap map = item.toMap();

            if(map.contains("type")) {
                QString typeAttachment = map.value("type").toString();

                if(typeAttachment == "photo") {
                    QVariantMap photoMap = map[ "photo" ].toMap();
                    if ( photoMap.contains( "src" ) )
                        m_data.insert( ImageUrl, photoMap.value( "src" ).toString() );
                }

                if(typeAttachment == "audio") {
                    QVariantMap audioMap = map[ "audio" ].toMap();

                    if(audioMap.contains("title"))
                        m_data.insert(Audio, audioMap.value("performer").toString() + " - " + audioMap.value("title").toString());
                }
            }
        }
    }

    if(map.contains("likes")) {
        QVariantMap likesMap = map[ "likes" ].toMap();
        if(likesMap.contains("count"))
            m_data.insert(Likes, likesMap.value("count").toString());
    }

    if(map.contains("comments")) {
        QVariantMap commentsMap = map[ "comments" ].toMap();
        if(commentsMap.contains("count"))
            m_data.insert(CommentCount, commentsMap.value("count").toString());
    }

    m_data.insert(PluginName, pluginName());
}
