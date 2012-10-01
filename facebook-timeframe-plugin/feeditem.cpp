#include "feeditem.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include <qjson/parser.h>


FeedItem::FeedItem(const QVariantMap &map, QString selfId) :
    m_selfId(selfId)
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
    if (isLink) {
        QString name = isLink ? map.value("name").toString() : message;
        m_data.insert(Text, QString("<a href=%1>%2</a>").arg(message, name));
    } else
        m_data.insert(Text, message);

    m_data.insert(ImageUrl, QUrl::fromPercentEncoding(map.value("picture").toByteArray()));
    if (message.isEmpty()) {
        m_data.insert(Text, map.value("story").toString());
    }
    if (map.contains("created_time")) {
        QDateTime dt = map.value("created_time").toDateTime();
        QDate date = dt.date();
        m_data.insert(Date, date.toString("d MM yyyy"));
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


