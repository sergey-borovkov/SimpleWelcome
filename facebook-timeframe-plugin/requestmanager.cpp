#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QStringList>

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

void RequestManager::feedReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }
    QVariantList list = result.value(QLatin1String("data")).toList();
    QList<SocialItem *> feedItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem(map, m_selfId);

        if (feedItem->data(SocialItem::Text).isNull()
                && feedItem->data(SocialItem::ImageUrl).isNull()) {
            delete feedItem;
        } else
            feedItems.append(feedItem);
    }

    emit newSocialItems(feedItems);

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
