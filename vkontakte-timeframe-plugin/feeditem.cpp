#include "feeditem.h"


#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtGui/QColor>
#include <QtGui/QPalette>

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

QDateTime FeedItem::datetime() const
{
    return data(DateTime).toDateTime();
}


static QRegExp rx_user_link("\\[(id\\d+)\\|(\\S*)\\]");

void FeedItem::fillFromMap(QVariantMap map)
{
    // http://vk.com/developers.php?oid=-1&p=%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D0%BE%D0%B2_API
    if (map.contains("id")) {
        m_id = map.value("id").toString();
    }

    QString message;
    if (map.contains("text")) {
        message = map.value("text").toString();

        QColor colorLink = QPalette().color(QPalette::Link);
        // if user posts a link
        QRegExp reUrl("(((?:https?|ftp)://|www)\\S+)");
        bool hasLink = message.contains(reUrl);
        if (hasLink) {
            int pos = reUrl.indexIn(message);
            QString after = "<a href=\"\\1\"><font color=\"" + colorLink.name() + "\">\\1</font></a>";
            if (pos > -1) {
                if (reUrl.cap(1).startsWith("www", Qt::CaseInsensitive)) {
                    after = "<a href=\"http://\\1\"><font color=\"" + colorLink.name() + "\">\\1</font></a>";
                }
            }

            message = message.replace(reUrl, after);
        }

        // if there is message with [user_id|user_name] string...
        int pos = rx_user_link.indexIn(message);
        if (pos != (-1)) {
            // get only user name
            message = rx_user_link.cap(2) + message.remove(rx_user_link.cap(0));
        }

        m_data.insert(Text, message);
    }

    if (map.contains("date")) {
        uint t  = map.value("date").toUInt();
        QDateTime dt;
        dt.setTime_t(t);
        m_data.insert(DateTime, dt);
    }

    m_data.insert(ImageUrl, QUrl(""));
    if (map.contains("attachments")) {
        QVariantList attachmentList = map[ "attachments" ].toList();

        foreach(QVariant item, attachmentList) {
            QVariantMap map = item.toMap();

            if (map.contains("type")) {
                QString typeAttachment = map.value("type").toString();

                // add image
                if (typeAttachment == "photo") {
                    QVariantMap photoMap = map[ "photo" ].toMap();
                    if (photoMap.contains("src"))
                        m_data.insert(ImageUrl, QUrl::fromPercentEncoding(photoMap.value("src").toByteArray()));
                }

                // add graffiti as image
                if (m_data[ImageUrl].toString().isEmpty() && typeAttachment == "graffiti" ) {
                    QVariantMap graffitiMap = map[ "graffiti" ].toMap();
                    if (graffitiMap.contains("src"))
                        m_data.insert(ImageUrl, QUrl::fromPercentEncoding(graffitiMap.value("src").toByteArray()));
                }

                // add audio info
                if (typeAttachment == "audio") {
                    QVariantMap audioMap = map[ "audio" ].toMap();

                    if (audioMap.contains("aid")) {
                        m_data.insert(AudioId, audioMap.value("aid").toString());
                    }

                    if (audioMap.contains("owner_id")) {
                        m_data.insert(AudioOwnerId, audioMap.value("owner_id").toString());
                    }

                    if (audioMap.contains("title")) {

                        QString audioStr = audioMap.value("performer").toString() + " - " +
                                audioMap.value("title").toString();

                        // add duration info (if possible)
                        if (audioMap.contains("duration")) {
                            audioStr += " (" + convertSecsToStr(audioMap.value("duration").toInt()) + ")";
                        }
                        m_data.insert(Audio, audioStr);
                    }
                }

                // add video info
                if (typeAttachment == "video") {
                    QVariantMap videoMap = map[ "video" ].toMap();

                    if (videoMap.contains("vid")) {
                        m_data.insert(VideoId, videoMap.value("vid").toString());
                    }

                    if (videoMap.contains("owner_id")) {
                        m_data.insert(VideoOwnerId, videoMap.value("owner_id").toString());
                    }

                    if (videoMap.contains("title")) {

                        QString videoStr = videoMap.value("title").toString();

                        // add duration info (if possible)
                        if (videoMap.contains("duration")) {
                            videoStr += " (" + convertSecsToStr(videoMap.value("duration").toInt()) + ")";
                        }

                        m_data.insert(Video, videoStr);
                    }
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

QString FeedItem::convertSecsToStr(int secs)
{
    QTime tm;
    tm = tm.addSecs(secs);
    QString str = "h:mm:ss";

    if (tm.hour() == 0) {
        str = "mm:ss";
        if (tm.minute() == 0) {
            str = "ss";
            if (tm.second() < 10)
                str = "s";
        }
        else if (tm.minute() < 10)
            str = "m:ss";
    }

    return tm.toString(str);
}

void fillCommentFromMap(CommentItem *item, const QVariantMap &map)
{
    uint t  = map.value("date").toUInt();
    QDateTime dt;
    dt.setTime_t(t);
    QDate date = dt.date();

    if (map.contains("reply_to_uid") && map.contains("reply_to_cid")) {
        QString str = map.value("text").toString();
        int pos = rx_user_link.indexIn(str);
        if (pos != (-1)) {
//            s = rx.cap(1); // user id
            // get only user name
            str = rx_user_link.cap(2) + str.remove(rx_user_link.cap(0));
        }
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
