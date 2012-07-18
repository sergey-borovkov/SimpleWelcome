#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

RequestManager::RequestManager(QObject *parent)
    : m_authorizer(0)
{
}

void RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    if(!m_authorizer)
        return;

    Request *request = new Request(m_authorizer->accessToken(), Request::WallPosts, this);

    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(reply(QByteArray)));
    request->startQuery();
}

void RequestManager::setAuthorizer(OAuth2Authorizer *authorizer)
{
    m_authorizer = authorizer;

    if(m_authorizer->isAuthorized())
        emit authorizationComplete();
    else
        connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SIGNAL(authorizationComplete()));
}

void RequestManager::logout()
{
    Request *request = new Request(m_authorizer->accessToken(), Request::Logout);
    request->startQuery();

    // actually first need to do some error checking
    m_authorizer->deauthorize();
}

FeedItem *RequestManager::parseReply(const QByteArray &reply)
{
    return NULL;
}

void RequestManager::reply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if(result.contains("error"))
    {
        m_authorizer->deauthorize();
        return;
    }

    QVariantList list = result.value("data").toList();

    QList<SocialItem *> feedItems;

    foreach(QVariant item, list)
    {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem();
        feedItem->fillFromMap(map);
        feedItems.append(feedItem);
    }
    emit newSocialItems(feedItems);
}
