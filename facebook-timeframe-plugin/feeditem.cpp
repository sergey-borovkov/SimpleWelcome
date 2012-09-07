#include "feeditem.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>

#include <qjson/parser.h>


FeedItem::FeedItem(const QVariantMap &map)
{
    fillFromMap(map);
}

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
    if(role == Id)
        return m_id;
    else
        return m_data.value(role);
}

QDate FeedItem::date() const
{
    return QDate::fromString(data(Date).toString(), QString("d MM yyyy"));
}

void FeedItem::fillFromMap(const QVariantMap &map)
{    
    // http://developers.facebook.com/docs/reference/api/post/
    m_id = map.value("id").toString();
    QString message = map.value("message").toString();

    // if user posts a link
    bool isLink = message.startsWith("http://");
    if(isLink) {
        QString name = isLink ? map.value("name").toString() : message;
        m_data.insert(Text, QString("<a href=%1>%2</a>").arg(message, name));
    } else
        m_data.insert(Text, message);

    m_data.insert( ImageUrl, map.value( "picture" ).toString() );
    if ( message.isEmpty() ) {
        m_data.insert( Text, map.value( "story" ).toString() );
    }
    if ( map.contains( "created_time" ) ) {
        QDateTime dt = map.value( "created_time" ).toDateTime();
        QDate date = dt.date();
        m_data.insert(Date, date.toString("d MM yyyy"));
    }

    if(map.contains("comments")) {
        QVariantMap comments = map.value("comments").toMap();
        if(comments.value("count").toInt() > 0)  {
            QVariantList commentsList = comments.value("data").toList();
            foreach(QVariant v, commentsList) {
                FacebookCommentItem *item = new FacebookCommentItem(v.toMap());
                m_comments.append(item);
            }
        }
    }

    if(map.contains("likes")) {
        QVariantMap likes = map.value("likes").toMap();
        m_data.insert(Likes, likes.value("count").toInt());
    } else
        m_data.insert(Likes, 0);


    QVariant var;
    var.setValue(m_comments);
    m_data.insert(CommentCount, m_comments.size());
    m_data.insert(Comments, var);

    m_data.insert(PluginName, pluginName());
}


FacebookCommentItem::FacebookCommentItem(const QVariantMap &map)
{
    fillFromMap(map);
}

QString FacebookCommentItem::id() const
{
    return data(Id).toString();
}

QVariant FacebookCommentItem::data(int role) const
{
    return m_data.value(role);
}

void FacebookCommentItem::fillFromMap(const QVariantMap &map)
{
    m_data.insert(Id, map.value("id"));
    m_data.insert(Message, map.value("message"));
    m_data.insert(CreatedTime, map.value("created_time"));
    if(map.contains("from")) {
        QVariantMap fromMap = map.value("from").toMap();
        m_data.insert(FromId, fromMap.value("id"));
        m_data.insert(From, fromMap.value("name"));
    }
}


