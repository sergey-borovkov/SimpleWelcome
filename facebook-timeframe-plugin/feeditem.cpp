#include "feeditem.h"

#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QRegExp>
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

void FeedItem::fillFromMap(const QVariantMap &map)
{
    // http://developers.facebook.com/docs/reference/api/post/
    m_id = map.value("id").toString();
    QString message = map.value("message").toString();

    QColor colorLink = QPalette().color(QPalette::Link);

    // if user posts a link
    QRegExp reUrl("(((?:https?|ftp)://|www)\\S+)");
    bool hasLink = message.contains(reUrl);
    if (hasLink) {
        int pos = reUrl.indexIn(message);

        QString after = "<a href=\"\\1\"><font color=\"" + colorLink.name() + "\">\\1</font></a>";
        if (pos > -1) {
            if (reUrl.cap(1).indexOf("www", Qt::CaseInsensitive) != (-1)) {
                after = "<a href=\"http://\\1\"><font color=\"" + colorLink.name() + "\">\\1</font></a>";
            }
        }

        m_data.insert(Text, message.replace(reUrl, after));
    }
    else
        m_data.insert(Text, message);

    m_data.insert(ImageUrl, QUrl::fromPercentEncoding(map.value("picture").toByteArray()));


    // get video if possible
    if (map.value("type").toString() == QLatin1String(QLatin1String("video")) &&
        !map.value(QLatin1String("source")).isNull()) {
        QString src = QUrl::fromPercentEncoding(map.value(QLatin1String("source")).toByteArray());
        if (!src.isEmpty()) {
            m_data.insert(VideoUrl, src);
            QString name = map.value("name").toString();
            if (!name.isEmpty())
                m_data.insert(Video, name);
            else
                m_data.insert(Video, src);
            m_data.insert(VideoImage, QUrl::fromPercentEncoding(map.value(QLatin1String("picture")).toByteArray()));
        }
    }

    if (map.contains("created_time")) {
        QDateTime dt = map.value("created_time").toDateTime();
        m_data.insert(DateTime, dt);
    }

    if (map.contains("comments")) {
        QVariantMap comments = map.value("comments").toMap();
        m_data.insert(CommentCount, comments.value("count").toInt());
        if (comments.value("count").toInt() > 0)  {
            QVariantList commentsList = comments.value("data").toList();
            foreach(QVariant v, commentsList) {
                CommentItem *item = new CommentItem();
                fillCommentFromMap(item, v.toMap());
                m_comments.append(item);
            }
            QList<ListItem *> t;
            foreach(CommentItem * item, m_comments)
            t.append(item);
            m_commentsModel->appendRows(t);
        }
    }

    if (map.contains("likes")) {
        QVariantMap likes = map.value("likes").toMap();
        if (likes.value("count").toInt() > 0) {
            QVariantList likesList = likes.value("data").toList();
            SocialItem::LikeType like = SocialItem::NotLiked;
            foreach(QVariant v, likesList) {
                QMap<QString, QVariant> map = v.toMap();
                if (map.value("id") == m_selfId)  {
                    like = SocialItem::Liked;
                    break;
                }
            }
            m_data.insert(Like, like);
        }
        m_data.insert(Likes, likes.value("count").toInt());
    } else
        m_data.insert(Likes, 0);

    QVariant var;
    var.setValue(m_commentsModel);
    m_data.insert(Comments, var);
    m_data.insert(PluginName, pluginName());
}

void fillCommentFromMap(CommentItem *item, const QVariantMap &map)
{
    item->setData(CommentItem::Id, map.value("id"));
    item->setData(CommentItem::Message, map.value("message"));
    item->setData(CommentItem::CreatedTime, map.value("created_time"));
    item->setData(CommentItem::Type, "Facebook");
    if (map.contains("from")) {
        QVariantMap fromMap = map.value("from").toMap();
        item->setData(CommentItem::FromId, fromMap.value("id"));
        item->setData(CommentItem::From, fromMap.value("name"));
    }
}


