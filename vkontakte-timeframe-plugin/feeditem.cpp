#include "feeditem.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QMap>

#include <qjson/parser.h>


FeedItem::FeedItem(const QVariantMap &map, QString selfId)
    : m_selfId(selfId)
{
    m_commentsModel = new ListModel(CommentItem::roleNames());
    fillFromMap(map);
}

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
    if (role == Id)
        return m_id;
    else
        return m_data.value(role);
}

bool FeedItem::setData(int role, const QVariant &value)
{
    m_data[role] = value;
    return true;
}

QDate FeedItem::date() const
{
    return QDate::fromString(data(Date).toString(), QString("d MM yyyy"));
}

void FeedItem::fillFromMap(QVariantMap map)
{
    // http://vk.com/developers.php?oid=-1&p=%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D0%BE%D0%B2_API
    if (map.contains("id")) {
        m_id = map.value("id").toString();
    }

    QString message;
    if (map.contains("text")) {
        message = map.value("text").toString();
    }

    // if user posts a link
    QRegExp reUrl("(((?:https?|ftp)://|www)\\S+)");
    bool hasLink = message.contains(reUrl);
    if (hasLink) {
        int pos = reUrl.indexIn(message);
        QString after = "<a href=\"\\1\">\\1</a>";
        if (pos > -1) {
            if (reUrl.cap(1).startsWith("www", Qt::CaseInsensitive)) {
                after = "<a href=\"http://\\1\">\\1</a>";
            }
        }

        m_data.insert(Text, message.replace(reUrl, after));
    }
    else
        m_data.insert(Text, message);

    if (map.contains("date")) {
        uint t  = map.value("date").toUInt();
        QDateTime dt;
        dt.setTime_t(t);
        QDate date = dt.date();
        m_data.insert(Date, date.toString("d MM yyyy"));
    }

    m_data.insert(ImageUrl, QUrl(""));
    if (map.contains("attachments")) {
        QVariantList attachmentList = map[ "attachments" ].toList();

        foreach(QVariant item, attachmentList) {
            QVariantMap map = item.toMap();

            if (map.contains("type")) {
                QString typeAttachment = map.value("type").toString();

                if (typeAttachment == "photo") {
                    QVariantMap photoMap = map[ "photo" ].toMap();
                    if (photoMap.contains("src"))
                        m_data.insert(ImageUrl, QUrl::fromPercentEncoding(photoMap.value("src").toByteArray()));
                }

                if (typeAttachment == "audio") {
                    QVariantMap audioMap = map[ "audio" ].toMap();

                    if (audioMap.contains("title"))
                        m_data.insert(Audio, audioMap.value("performer").toString() + " - " + audioMap.value("title").toString());
                }
            }
        }
    }

    if (map.contains("likes")) {
        QVariantMap likesMap = map[ "likes" ].toMap();
        if (likesMap.contains("count")) {
            m_data.insert(Likes, likesMap.value("count").toString());
        }
    }
    else
        m_data.insert(Likes, 0);
    m_data.insert(Like, SocialItem::NotLiked);

    if (map.contains("comments")) {
        QVariantMap commentsMap = map[ "comments" ].toMap();
        if (commentsMap.contains("count"))
            m_data.insert(CommentCount, commentsMap.value("count").toString());
    }
    else
        m_data.insert(CommentCount, 0);

    QVariant var;
    var.setValue(m_commentsModel);
    m_data.insert(Comments, var);
    m_data.insert(PluginName, pluginName());
}

void fillCommentFromMap(CommentItem *item, const QVariantMap &map)
{
    uint t  = map.value("date").toUInt();
    QDateTime dt;
    dt.setTime_t(t);
    QDate date = dt.date();

    if (map.contains("reply_to_uid") && map.contains("reply_to_cid")) {
        QString s, str = map.value("text").toString();
        QRegExp rx("\\[(id\\d+)\\|(\\S*)\\]");
        int pos = rx.indexIn(str);
        if (pos != (-1)) {
//            s = rx.cap(1); // user id
            s = rx.cap(2); // user name
        }
        str = s + str.remove(rx.cap(0));
        item->setData(CommentItem::Message, str);
    }
    else {
        item->setData(CommentItem::Message, map.value("text"));
    }

    item->setData(CommentItem::Id, map.value("cid"));
    item->setData(CommentItem::CreatedTime, date.toString("d MM yyyy"));
    item->setData(CommentItem::FromId, map.value("uid"));
    item->setData(CommentItem::Type, "VKontakte");
    item->setData(CommentItem::From, "");
}
