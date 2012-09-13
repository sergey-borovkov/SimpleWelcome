#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

RequestManager::RequestManager(QObject *parent)
    : QObject(parent)
    , m_authorizer(0)
{
}

Request *RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    VkRequest *request = new VkRequest(m_authorizer->accessToken(), VkRequest::WallPosts, this, 0);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(reply(QByteArray)));
    return request;
}

Request *RequestManager::queryUserId()
{
    return 0;
}

Request *RequestManager::queryImage(const QString &id)
{
    Q_UNUSED(id)
    VkRequest *request = new VkRequest(m_authorizer->accessToken(), VkRequest::Image, this, 0);

}

Request *RequestManager::postComment(const QString &message, const QString &parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(message)
}

Request *RequestManager::like(const QString &id)
{
    Q_UNUSED(id)
    VkRequest *request = new VkRequest(m_authorizer->accessToken(), VkRequest::Like);
    return request;
}

void RequestManager::setAuthorizer(OAuth2Authorizer *authorizer)
{
    m_authorizer = authorizer;

    if(m_authorizer->isAuthorized())
        emit authorizationComplete();
    else
        connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SIGNAL(authorizationComplete()));
}

Request *RequestManager::logout()
{
    VkRequest *request = new VkRequest(m_authorizer->accessToken(), VkRequest::Logout);
    connect(request, SIGNAL(success()), m_authorizer, SLOT(logout()));

    // actually first need to do some error checking
    m_authorizer->logout();
    return request;
}

void RequestManager::reply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if(result.contains("error")) {
        m_authorizer->logout();
        return;
    }

    int itemCount = 0;
    QVariantList list = result.value("response").toList();
    if(list.size() > 1) {
        itemCount = list.at(0).toInt();
        // delete first item from list because first item is a count of messages in the wall
        list.takeFirst();
    }

    int cycles = 0;
    if(itemCount >= 100) {
        cycles = itemCount / 100;
    }

    // need repeat query
    if(cycles) {
        for(int i = 0; i < cycles; i++) {
            VkRequest *request = new VkRequest(m_authorizer->accessToken(), VkRequest::WallPosts, this, 100 *(i + 1));
            connect(request, SIGNAL(replyReady(QByteArray)), SLOT(replyQueryWall(QByteArray)));
            request->start();
        }
    }

    QList<SocialItem *> feedItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem();
        feedItem->fillFromMap(map);
        feedItems.append(feedItem);
    }
    emit newSocialItems(feedItems);
}


void RequestManager::replyQueryWall(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if(result.contains("error")) {
        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value("response").toList();
    if(list.size() > 1) {
        list.takeFirst();
    }

    QList<SocialItem *> feedItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem();
        feedItem->fillFromMap(map);
        feedItems.append(feedItem);
    }

    emit newSocialItems(feedItems);
}
