#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

RequestManager::RequestManager(QObject *parent)
    : QObject(parent)
    , m_authorizer(0)
    , m_gotCommentsCount(0)
    , m_gotLikesCount(0)
    , m_gotMessagesCount(0)
    , m_requestItemCount(100)
{
}

Request *RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    Q_UNUSED(beginDate)
    Q_UNUSED(endDate)

    QUrl url = constructUrl(QLatin1String("wall.get"));
    url.addQueryItem(QLatin1String("offset"), QLatin1String("0"));
    url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));
    url.addQueryItem(QLatin1String("filter"), QLatin1String("all"));

    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(feedReply(QByteArray)));
    request->setUrl(url);

    return request;
}

Request *RequestManager::queryUserId()
{
    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(idReply(QByteArray)));
    request->setUrl(constructUrl(QLatin1String("getUserInfoEx")));
    return request;
}

Request *RequestManager::queryImage(const QString &id)
{
    QUrl url = constructUrl(QLatin1String("getProfiles"));
    url.addQueryItem(QLatin1String("uids"), id);
    url.addQueryItem(QLatin1String("fields"), QLatin1String("photo,first_name,last_name,nickname"));
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());

    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(imageReply(QByteArray)));
    request->setUrl(url);
    return request;
}

Request *RequestManager::queryAudio(const QString &aid, const QString &ownerId)
{
    QUrl url = constructUrl(QLatin1String("audio.getById"));
    url.addQueryItem(QLatin1String("audios"), ownerId + QLatin1String("_") + aid);

    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(audioReply(QByteArray)));

    request->setUrl(url);
    return request;
}

Request *RequestManager::queryVideo(const QString &vid, const QString &ownerId)
{
    QUrl url = constructUrl(QLatin1String("video.get"));
    url.addQueryItem(QLatin1String("videos"), ownerId + QLatin1String("_") + vid);

    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(videoReply(QByteArray)));

    request->setUrl(url);
    return request;
}

Request *RequestManager::queryUserInfo(const QString &fromId)
{
    QUrl url = constructUrl(QLatin1String("users.get"));
    url.addQueryItem(QLatin1String("uids"), fromId);
    url.addQueryItem(QLatin1String("fields"), QLatin1String("photo,first_name,last_name,nickname"));

    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(userInfoReply(QByteArray)));

    request->setUrl(url);
    return request;
}

Request *RequestManager::postComment(const QByteArray &message, const QString &postId)
{
    QUrl url = constructUrl(QLatin1String("wall.addComment"));
    url.addQueryItem(QLatin1String("post_id"), postId);
    url.addEncodedQueryItem("text", message);

    VkRequest *request = new VkRequest(VkRequest::Post, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(postCommentReply(QByteArray)));
    request->setUrl(url);

    return request;
}

Request *RequestManager::like(const QString &id)
{
    VkRequest *request = new VkRequest(VkRequest::Post, this);

    QUrl url = constructUrl(QLatin1String("wall.addLike"));
    url.addQueryItem(QLatin1String("post_id"), id);

    request->setUrl(url);

    return request;
}

Request *RequestManager::unlike(const QString &id)
{
    VkRequest *request = new VkRequest(VkRequest::Delete, this);
    QUrl url = constructUrl(QLatin1String("wall.deleteLike"));
    url.addQueryItem(QLatin1String("post_id"), id);

    request->setUrl(url);

    return request;
}

Request *RequestManager::queryComments(const QString &postId)
{
    VkRequest *request = new VkRequest(VkRequest::Get, this);
    QUrl url = constructUrl(QLatin1String("wall.getComments"));
    url.addQueryItem(QLatin1String("post_id"), postId);
    url.addQueryItem(QLatin1String("sort"), QLatin1String("asc"));
    url.addQueryItem(QLatin1String("offset"), QLatin1String("0"));
    url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));
    url.addQueryItem(QLatin1String("preview_length"), QLatin1String("0"));

    request->setUrl(url);

    // remember id so that slot can know it
    request->setProperty("postId", postId);

    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(commentReply(QByteArray)));

    return request;
}

Request *RequestManager::queryLikes(const QString &postId)
{
    VkRequest *request = new VkRequest(VkRequest::Get, this);
    QUrl url = constructUrl(QLatin1String("wall.getLikes"));
    url.addQueryItem(QLatin1String("post_id"), postId);
    url.addQueryItem(QLatin1String("offset"), QLatin1String("0"));
    url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));

    request->setUrl(url);

    // remember id so that slot can know it
    request->setProperty("postId", postId);

    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(likesReply(QByteArray)));

    return request;
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
    VkRequest *request = new VkRequest(VkRequest::Get, this);
    connect(request, SIGNAL(success()), m_authorizer, SLOT(logout()));

    QUrl url(QLatin1String("http://oauth.vk.com/oauth/logout"));
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

    int messagesCount = 0;
    QVariantList list = result.value(QLatin1String("response")).toList();
    if (list.size() > 1) {
        messagesCount = list.at(0).toInt();
        // delete first item from list because first item is a count of messages in the wall
        list.takeFirst();
    }

    if (messagesCount == 0)
        return;

    m_gotMessagesCount += list.size();

    QList<SocialItem *> feedItems;
    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem(map, m_selfId);
        if (!canBeDisplayed(*feedItem)) {
            delete feedItem;
        }
        else {
            feedItems.append(feedItem);
        }
    }

    emit newSocialItems(feedItems);

    if (messagesCount > m_gotMessagesCount) {
        QUrl url = constructUrl(QLatin1String("wall.get"));
        url.addQueryItem(QLatin1String("offset"), QString("%1").arg(m_gotMessagesCount));
        url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));
        url.addQueryItem(QLatin1String("filter"), QLatin1String("all"));

        VkRequest *request = new VkRequest(VkRequest::Get, this);
        connect(request, SIGNAL(replyReady(QByteArray)), SLOT(feedReply(QByteArray)));
        request->setUrl(url);
        request->start();
    }
    else {
        m_gotMessagesCount = 0;
        emit searchComplete();
    }
}

void RequestManager::replyQueryWall(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains("error")) {
        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value("response").toList();
    if (list.size() > 1) {
        list.takeFirst();   // it's "count"
    }

    QList<SocialItem *> feedItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem(map, m_selfId);

        if (!canBeDisplayed(*feedItem))
            delete feedItem;
        else
            feedItems.append(feedItem);
    }

    emit newSocialItems(feedItems);
}

void RequestManager::commentReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        return;
    }

    int commentCount = 0;
    QVariantList list = result.value(QLatin1String("response")).toList();
    if (list.size() > 1) {
        commentCount = list.at(0).toInt();
        // delete first item from list because first item is a count of messages in the wall
        list.takeFirst();
    }

    if (commentCount == 0)
        return;

    m_gotCommentsCount += list.size();

    QList<CommentItem *> comments;
    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        CommentItem *commentItem = new CommentItem();
        fillCommentFromMap(commentItem, map);
        comments.append(commentItem);
    }

    QString postId = sender()->property("postId").toString();
    QList<CommentItem *> cachedComments = m_comments.value(postId);
    cachedComments.append(comments);

    if (commentCount > m_gotCommentsCount) {
        m_comments.insert(postId, cachedComments);

        VkRequest *request = new VkRequest(VkRequest::Get, this);
        QUrl url = constructUrl(QLatin1String("wall.getComments"));
        url.addQueryItem(QLatin1String("post_id"), postId);
        url.addQueryItem(QLatin1String("sort"), QLatin1String("asc"));
        url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));
        url.addQueryItem(QLatin1String("offset"), QString("%1").arg(m_gotCommentsCount));
        url.addQueryItem(QLatin1String("preview_length"), QLatin1String("0"));

        connect(request, SIGNAL(replyReady(QByteArray)), SLOT(commentReply(QByteArray)));
        request->setProperty("postId", postId);
        request->setUrl(url);
        request->start();
    }
    else {
        m_comments.remove(postId);
        m_gotCommentsCount = 0;
        emit newComments(postId, cachedComments);
    }
}

void RequestManager::likesReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();
    if (result.contains(QLatin1String("error"))) {
        return;
    }

    result = result.value(QLatin1String("response")).toMap();

    int likesCount = result.value("count").toInt();
    if (likesCount == 0)
        return;

    QVariantList list = result.value(QLatin1String("users")).toList();

    m_gotLikesCount += list.size();

    bool isSelfLiked = false;
    QString postId = sender()->property("postId").toString();

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        QString uid = map.value("uid").toString();
        if (uid == m_selfId) {
            isSelfLiked = true;
            emit selfLiked(postId);
            m_gotLikesCount = 0;
            break;
        }
    }

    if (!isSelfLiked && (likesCount > m_gotLikesCount)) {
        VkRequest *request = new VkRequest(VkRequest::Get, this);
        QUrl url = constructUrl(QLatin1String("wall.getLikes"));
        url.addQueryItem(QLatin1String("post_id"), postId);
        url.addQueryItem(QLatin1String("count"), QString("%1").arg(m_requestItemCount));
        url.addQueryItem(QLatin1String("offset"), QString("%1").arg(m_gotLikesCount));

        connect(request, SIGNAL(replyReady(QByteArray)), SLOT(likesReply(QByteArray)));
        request->setProperty("postId", postId);
        request->setUrl(url);
        request->start();
    }
    else {
        m_gotLikesCount = 0;
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

    if (result.contains(QLatin1String("response"))) {
        QVariantMap map = result.value(QLatin1String("response")).toMap();

        m_selfId = map.value(QLatin1String("user_id")).toString();
        emit selfId(m_selfId);

        m_selfName = map.value(QLatin1String("user_name")).toString();
        emit selfName(m_selfName);
    }
}

void RequestManager::imageReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    QString userId, userImageUrl, userName;

    if (result.contains(QLatin1String("response"))) {
        QVariantList list = result.value(QLatin1String("response")).toList();

        foreach(QVariant item, list) {
            QVariantMap map = item.toMap();

            if (map.contains(QLatin1String("uid"))) {
                userId = map.value(QLatin1String("uid")).toString();
            }
            if (map.contains(QLatin1String("photo"))) {
                userImageUrl = map.value(QLatin1String("photo")).toString();
            }
            if (map.contains(QLatin1String("first_name"))) {
                userName += map.value(QLatin1String("first_name")).toString() + " ";
            }
            if (map.contains(QLatin1String("last_name"))) {
                userName += map.value(QLatin1String("last_name")).toString();
            }
            if (userName.isEmpty() && map.contains(QLatin1String("nickname"))) {
                userName += map.value(QLatin1String("nickname")).toString();
            }

            emit gotUserImage(userId, userImageUrl);
            emit gotUserName(userId, userName);
        }
    }
}

void RequestManager::audioReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    QString audioId, audioOwnerId, audioUrl;

    if (result.contains(QLatin1String("response"))) {
        QVariantList list = result.value(QLatin1String("response")).toList();

        foreach(QVariant item, list) {
            QVariantMap map = item.toMap();

            if (map.contains(QLatin1String("aid"))) {
                audioId = map.value(QLatin1String("aid")).toString();
            }
            if (map.contains(QLatin1String("owner_id"))) {
                audioOwnerId = map.value(QLatin1String("owner_id")).toString();
            }
            if (map.contains(QLatin1String("url"))) {
                audioUrl = map.value(QLatin1String("url")).toString();
            }

            emit gotAudioUrl(audioId, audioOwnerId, audioUrl);
        }
    }
}

void RequestManager::videoReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    // video fields: vid, owner_id, title, description, duration, link, image, date, player
    QString videoId, videoOwnerId, videoUrl, videoImage;

    if (result.contains(QLatin1String("response"))) {
        QVariantList list = result.value(QLatin1String("response")).toList();

        foreach(QVariant item, list) {
            QVariantMap map = item.toMap();

            if (map.contains(QLatin1String("vid"))) {
                videoId = map.value(QLatin1String("vid")).toString();
            }
            if (map.contains(QLatin1String("owner_id"))) {
                videoOwnerId = map.value(QLatin1String("owner_id")).toString();
            }
            if (map.contains(QLatin1String("player"))) {
                videoUrl = QUrl::fromPercentEncoding(map.value(QLatin1String("player")).toByteArray());
            }
            if (map.contains(QLatin1String("image"))) {
                videoImage = map.value(QLatin1String("image")).toString();
            }
            if (!videoId.isEmpty() && !videoOwnerId.isEmpty()
                    && !videoId.isEmpty() && !videoImage.isEmpty()) {
                emit gotVideoUrl(videoId, videoOwnerId, videoUrl, videoImage);
            }
        }
    }
}

void RequestManager::userInfoReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    // video fields: vid, owner_id, title, description, duration, link, image, date, player
    QString userId, userName, userImageUrl;

    if (result.contains(QLatin1String("response"))) {
        QVariantList list = result.value(QLatin1String("response")).toList();

        foreach(QVariant item, list) {
            QVariantMap map = item.toMap();

            if (map.contains(QLatin1String("uid"))) {
                userId = map.value(QLatin1String("uid")).toString();
            }
            if (map.contains(QLatin1String("first_name"))) {
                userName = map.value(QLatin1String("first_name")).toString();
            }
            if (map.contains(QLatin1String("last_name"))) {
                if (!userName.isEmpty())
                    userName += " ";
                userName += map.value(QLatin1String("last_name")).toString();
            }
            if (userName.isEmpty() && map.contains(QLatin1String("nickname"))) {
                userName = map.value(QLatin1String("nickname")).toString();
            }
            if (map.contains(QLatin1String("photo"))) {
                userImageUrl = map.value(QLatin1String("photo")).toString();
            }
            if (!userId.isEmpty() && !userName.isEmpty()&& !userImageUrl.isEmpty()) {
//                qDebug() << "[VK]   RequestManager::userInfoReply:   OK - " << userId << userName << userImageUrl;
                emit gotUserInfo(userId, userName, userImageUrl);
            }
        }
    }
}

void RequestManager::postCommentReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    if (result.contains(QLatin1String("response"))) {
    }
}

QUrl RequestManager::constructUrl(const QString &id) const
{
    QUrl url = apiVkUrl + id;
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());
    return url;
}

bool RequestManager::canBeDisplayed(const FeedItem &feedItem) const
{
    return !(feedItem.data(SocialItem::Text).toString().isEmpty() &&
             feedItem.data(SocialItem::ImageUrl).toString().isEmpty() &&
             feedItem.data(SocialItem::Audio).toString().isEmpty() &&
             feedItem.data(SocialItem::Video).toString().isEmpty());
}
