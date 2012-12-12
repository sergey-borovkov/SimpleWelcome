#include "request.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include "socialitem.h"
#include <commentitem.h>
#include <listmodel.h>

#include <qjson/parser.h>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

RequestManager::RequestManager(QObject *parent)
    : QObject(parent)
    , m_authorizer(0)
{
}

Request *RequestManager::queryWall()
{
    QUrl url = constructUrl(QLatin1String("me"), QLatin1String("feed"));
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(feedReply(QByteArray)));
    request->setUrl(url);

    return request;
}

Request *RequestManager::queryUserId()
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(idReply(QByteArray)));
    request->setUrl(constructUrl(QLatin1String("me"), ""));
    return request;
}

Request *RequestManager::queryImage(const QString &id)
{
    QUrl url = openGraphUrl + id;
    url.addQueryItem(QLatin1String("fields"), QLatin1String("picture"));
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());

    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(imageReply(QByteArray)));
    request->setUrl(url);
    return request;
}

Request *RequestManager::queryAudio(const QString &aid, const QString &ownerId)
{
    Q_UNUSED(ownerId)
    Q_UNUSED(aid)
    return 0;
}

Request *RequestManager::queryVideo(const QString &vid, const QString &ownerId)
{
    Q_UNUSED(ownerId)
    Q_UNUSED(vid)
    return 0;
}

Request *RequestManager::queryUserInfo(const QString &fromId)
{
    return queryImage(fromId);
}

Request *RequestManager::postComment(const QByteArray &message, const QString &parentId)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Post, this);
    QUrl url = constructUrl(parentId, QLatin1String("comments"));
    url.addEncodedQueryItem("message", message);
    request->setUrl(url);

    return request;
}

Request *RequestManager::postToWall(const QByteArray &message)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Post, this);
    QUrl url = constructUrl(QLatin1String("me"), QLatin1String("feed"));
    url.addEncodedQueryItem("message", message);
    request->setUrl(url);

    return request;
}

Request *RequestManager::like(const QString &id)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Post, this);
    request->setUrl(constructUrl(id, QLatin1String("likes")));
    return request;
}

Request *RequestManager::unlike(const QString &id)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Delete, this);
    request->setUrl(constructUrl(id, QLatin1String("likes")));
    return request;
}

Request *RequestManager::queryComments(const QString &postId)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    request->setUrl(constructUrl(postId, QLatin1String("comments")));

    // remember id so that slot can know it
    request->setProperty("postId", postId);

    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(commentReply(QByteArray)));

    return request;
}

Request *RequestManager::queryLikes(const QString &postId)
{
    Q_UNUSED(postId)
    return 0;
}

void RequestManager::setAuthorizer(OAuth2Authorizer *authorizer)
{
    m_authorizer = authorizer;

    if (m_authorizer->isAuthorized())
        emit authorizationComplete();
    else
        connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SIGNAL(authorizationComplete()));
}

Request *RequestManager::logout()
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    connect(request, SIGNAL(success()), m_authorizer, SLOT(logout()));

    QUrl url(QLatin1String("https://www.facebook.com/logout.php"));
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());

    m_authorizer->logout();
    return request;
}

QString RequestManager::pluginName() const
{
    return QLatin1String("Facebook");
}

void RequestManager::feedReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }
    QVariantList list = result.value(QLatin1String("data")).toList();
    QList<SocialItem *> socialItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        SocialItem *socialItem = new SocialItem(m_selfId);
        fillFromMap(socialItem, map);

        if (socialItem->data(SocialItem::Text).isNull()
                && socialItem->data(SocialItem::ImageUrl).isNull()) {
            delete socialItem;
        } else
            socialItems.append(socialItem);
    }

    emit newSocialItems(socialItems);

    QVariantMap paging = result.value(QLatin1String("paging")).toMap();
    if (paging.contains("next")) {
        FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
        connect(request, SIGNAL(replyReady(QByteArray)), SLOT(feedReply(QByteArray)));
        request->setUrl(paging.value("next").toUrl());
        request->start();
    } else {
        emit searchComplete();
    }
}

void RequestManager::commentReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        return;
    }

    QList<CommentItem *> comments;
    QVariantList list = result.value(QLatin1String("data")).toList();
    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        CommentItem *commentItem = new CommentItem();
        fillCommentFromMap(commentItem, map);
        comments.append(commentItem);
    }

    QString id = sender()->property("postId").toString();
    QList<CommentItem *> cachedComments = m_comments.value(id);
    cachedComments.append(comments);

    QVariantMap paging = result.value(QLatin1String("paging")).toMap();
    if (paging.contains("next")) {
        m_comments.insert(id, cachedComments);
        FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
        request->setProperty("postId", id);
        connect(request, SIGNAL(replyReady(QByteArray)), SLOT(commentReply(QByteArray)));
        request->setUrl(paging.value("next").toUrl());
        request->start();
    } else {
        m_comments.remove(id);
        emit newComments(id, cachedComments);
    }
}

void RequestManager::idReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    m_selfId = result.value(QLatin1String("id")).toString();
    emit selfId(m_selfId);

    m_selfName = result.value(QLatin1String("name")).toString();
    emit selfName(m_selfName);
}

void RequestManager::imageReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    QString userId, userImageUrl;

    if (result.contains(QLatin1String("picture"))) {
        userId = result.value(QLatin1String("id")).toString();
        QVariantMap map = result.value(QLatin1String("picture")).toMap();
        if (map.contains("data")) {
            map = map.value("data").toMap();
            if (map.contains("url")) {
                userImageUrl = map.value(QLatin1String("url")).toString();
                emit gotUserImage(userId, userImageUrl);
                emit gotUserInfo(userId, "", userImageUrl);
            }
        } else {
            userImageUrl = result.value(QLatin1String("picture")).toString();
            userId = result.value(QLatin1String("id")).toString();
            emit gotUserImage(userId, userImageUrl);
            emit gotUserInfo(userId, "", userImageUrl);
        }
    }
}

QUrl RequestManager::constructUrl(const QString &id, const QString &type) const
{
    QUrl url = openGraphUrl + id + "/" + type;
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());
    return url;
}

void RequestManager::fillFromMap(SocialItem *item, const QVariantMap &map)
{
    // http://developers.facebook.com/docs/reference/api/post/
    item->setId(map.value("id").toString());
    QString message = map.value("message").toString();

    QColor colorLink("#84c0ea");

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

        item->setData(SocialItem::Text, message.replace(reUrl, after));
    }
    else
        item->setData(SocialItem::Text, message);

    // get information about user who posted the message
    if (map.contains("from")) {
        QVariantMap fromInfo = map.value("from").toMap();
        item->setData(SocialItem::FromId, fromInfo.value("id").toString());
        item->setData(SocialItem::FromName, fromInfo.value("name").toString());
    }

    item->setData(SocialItem::ImageUrl, QUrl::fromPercentEncoding(map.value("picture").toByteArray()));

    // get video if possible
    if (map.value("type").toString() == QLatin1String(QLatin1String("video")) &&
        !map.value(QLatin1String("source")).isNull()) {
        QString src = QUrl::fromPercentEncoding(map.value(QLatin1String("source")).toByteArray());
        if (!src.isEmpty()) {
            item->setData(SocialItem::VideoUrl, src);
            QString name = map.value("name").toString();
            if (!name.isEmpty())
                item->setData(SocialItem::Video, name);
            else
                item->setData(SocialItem::Video, src);
            item->setData(SocialItem::VideoImage, QUrl::fromPercentEncoding(map.value(QLatin1String("picture")).toByteArray()));
            item->setData(SocialItem::ImageUrl, ""); //Set null to common image
        }
    }

    if (map.contains("created_time")) {
        QDateTime dt = map.value("created_time").toDateTime();
        item->setData(SocialItem::DateTime, dt);
    }

    if (map.contains("comments")) {
        QVariantMap commentsMap = map.value("comments").toMap();
        item->setData(SocialItem::CommentCount, commentsMap.value("count").toInt());
        if (commentsMap.value("count").toInt() > 0)  {
            QVariantList commentsList = commentsMap.value("data").toList();

            QList<CommentItem *> comments;
            foreach(QVariant v, commentsList) {
                CommentItem *commentItem = new CommentItem();
                fillCommentFromMap(commentItem, v.toMap());
                comments.append(commentItem);
            }

            QVariant var = item->data(SocialItem::Comments);
            ListModel *commentsModel = qvariant_cast<ListModel * >(var);
            if (commentsModel) {
                QList<ListItem *> t;
                foreach(CommentItem * item, comments)
                    t.append(item);
                commentsModel->appendRows(t);
            }
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
            item->setData(SocialItem::Like, like);
        }
        item->setData(SocialItem::Likes, likes.value("count").toInt());
    } else
        item->setData(SocialItem::Likes, 0);

    item->setData(SocialItem::PluginName, pluginName());
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
