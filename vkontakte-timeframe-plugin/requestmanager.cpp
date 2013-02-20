/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Julia Mineeva <julia.mineeva@osinit.ru>
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "request.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <socialitem.h>
#include <commentitem.h>

#include <qjson/parser.h>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

RequestManager::RequestManager(QObject *parent)
    : QObject(parent)
    , m_authorizer(0)
    , m_gotCommentsCount(0)
    , m_gotLikesCount(0)
    , m_gotMessagesCount(0)
{
}

Request *RequestManager::queryWall()
{
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

Request *RequestManager::postToWall(const QByteArray &message)
{
    QUrl url = constructUrl(QLatin1String("wall.post"));
    url.addEncodedQueryItem("message", message);

    VkRequest *request = new VkRequest(VkRequest::Post, this);
    // TODO: proper error handling
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(postMessageReply(QByteArray)));
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
//        m_authorizer->logout();
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

    QList<SocialItem *> socialItems;
    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        SocialItem *socialItem = new SocialItem(m_selfId);
        fillFromMap(socialItem, map);
        if (!canBeDisplayed(*socialItem)) {
            delete socialItem;
        }
        else {
            socialItems.append(socialItem);
        }
    }

    emit newSocialItems(socialItems);

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
//        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value("response").toList();
    if (list.size() > 1) {
        list.takeFirst();   // it's "count"
    }

    QList<SocialItem *> socialItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        SocialItem *socialItem = new SocialItem(m_selfId);
        fillFromMap(socialItem, map);

        if (!canBeDisplayed(*socialItem))
            delete socialItem;
        else
            socialItems.append(socialItem);
    }

    emit newSocialItems(socialItems);
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
//        m_authorizer->logout();
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
//        m_authorizer->logout();
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
//        m_authorizer->logout();
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
//        m_authorizer->logout();
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
//        m_authorizer->logout();
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
//        m_authorizer->logout();
        return;
    }
}

void RequestManager::postMessageReply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if (result.contains(QLatin1String("error"))) {
//        m_authorizer->logout();
        return;
    }
}

QUrl RequestManager::constructUrl(const QString &id) const
{
    QUrl url = apiVkUrl + id;
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());
    return url;
}

bool RequestManager::canBeDisplayed(const SocialItem &socialItem) const
{
    return !(socialItem.data(SocialItem::Text).toString().isEmpty() &&
             socialItem.data(SocialItem::ImageUrl).toString().isEmpty() &&
             socialItem.data(SocialItem::Audio).toString().isEmpty() &&
             socialItem.data(SocialItem::Video).toString().isEmpty());
}

static QRegExp rx_user_link("\\[(id\\d+)\\|(\\S*)\\]");

void RequestManager::fillFromMap(SocialItem *socialItem, QVariantMap map)
{
    // http://vk.com/developers.php?oid=-1&p=%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4%D0%BE%D0%B2_API
    if (map.contains("id")) {
        socialItem->setId(map.value("id").toString());
    }

    // get id for user who posted the message
    if (map.contains("from_id")) {
        socialItem->setData(SocialItem::FromId, map.value("from_id").toString());
    }

    QString message;
    if (map.contains("text")) {
        message = map.value("text").toString();

        QColor colorLink("#84c0ea");
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

        socialItem->setData(SocialItem::Text, message);
    }

    if (map.contains("date")) {
        uint t  = map.value("date").toUInt();
        QDateTime dt;
        dt.setTime_t(t);
        socialItem->setData(SocialItem::DateTime, dt);
    }

    socialItem->setData(SocialItem::ImageUrl, QUrl(""));
    if (map.contains("attachments")) {
        QVariantList attachmentList = map[ "attachments" ].toList();

        foreach(QVariant item, attachmentList) {
            QVariantMap map = item.toMap();

            if (map.contains("type")) {
                QString typeAttachment = map.value("type").toString();

                // add image
                if (typeAttachment == "photo") {
                    QVariantMap photoMap = map[ "photo" ].toMap();
                    if (photoMap.contains("src_big"))
                        socialItem->setData(SocialItem::ImageUrl, QUrl::fromPercentEncoding(photoMap.value("src_big").toByteArray()));
                    else if (photoMap.contains("src"))
                        socialItem->setData(SocialItem::ImageUrl, QUrl::fromPercentEncoding(photoMap.value("src").toByteArray()));
                }

                // add graffiti as image
                if (socialItem->data(SocialItem::ImageUrl).toString().isEmpty() && typeAttachment == "graffiti") {
                    QVariantMap graffitiMap = map[ "graffiti" ].toMap();
                    if (graffitiMap.contains("src"))
                        socialItem->setData(SocialItem::ImageUrl, QUrl::fromPercentEncoding(graffitiMap.value("src").toByteArray()));
                }

                // add audio info
                if (typeAttachment == "audio") {
                    QVariantMap audioMap = map[ "audio" ].toMap();

                    if (audioMap.contains("aid")) {
                        socialItem->setData(SocialItem::AudioId, audioMap.value("aid").toString());
                    }

                    if (audioMap.contains("owner_id")) {
                        socialItem->setData(SocialItem::AudioOwnerId, audioMap.value("owner_id").toString());
                    }

                    if (audioMap.contains("title")) {

                        QString audioStr = audioMap.value("performer").toString() + " - " +
                                           audioMap.value("title").toString();

                        // add duration info (if possible)
                        if (audioMap.contains("duration")) {
                            audioStr += " (" + convertSecsToStr(audioMap.value("duration").toInt()) + ")";
                        }
                        if (!audioStr.isEmpty()) {
                            socialItem->setData(SocialItem::Audio, audioStr);
                        }
                    }
                }

                // add video info
                if (typeAttachment == "video") {
                    QVariantMap videoMap = map[ "video" ].toMap();

                    if (videoMap.contains("vid")) {
                        socialItem->setData(SocialItem::VideoId, videoMap.value("vid").toString());
                    }

                    if (videoMap.contains("owner_id")) {
                        socialItem->setData(SocialItem::VideoOwnerId, videoMap.value("owner_id").toString());
                    }

                    if (videoMap.contains("title")) {

                        QString videoStr = videoMap.value("title").toString();

                        // add duration info (if possible)
                        if (videoMap.contains("duration")) {
                            videoStr += " (" + convertSecsToStr(videoMap.value("duration").toInt()) + ")";
                        }

                        if (!videoStr.isEmpty()) {
                            socialItem->setData(SocialItem::Video, videoStr);
                        }
                    }
                }
            }
        }
    }

    if (map.contains("likes")) {
        QVariantMap likesMap = map[ "likes" ].toMap();
        if (likesMap.contains("count")) {
            socialItem->setData(SocialItem::Likes, likesMap.value("count").toString());
        }
    }
    else
        socialItem->setData(SocialItem::Likes, 0);
    socialItem->setData(SocialItem::Like, SocialItem::NotLiked);

    if (map.contains("comments")) {
        QVariantMap commentsMap = map[ "comments" ].toMap();
        if (commentsMap.contains("count"))
            socialItem->setData(SocialItem::CommentCount, commentsMap.value("count").toString());
    }
    else
        socialItem->setData(SocialItem::CommentCount, 0);

    socialItem->setData(SocialItem::PluginName, pluginName());
}

QString convertSecsToStr(int secs)
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


QString RequestManager::pluginName()
{
    return QLatin1String("VKontakte");
}
