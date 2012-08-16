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

void RequestManager::queryImage(const QString &id)
{
    Q_UNUSED(id)
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
    qDebug() << "Logging out...";
    Request *request = new Request(m_authorizer->accessToken(), Request::Logout);
    connect(request, SIGNAL(success()), m_authorizer, SLOT(logout()));
    request->startQuery();

    // actually first need to do some error checking
    m_authorizer->logout();
}

void RequestManager::reply(QByteArray reply)
{
    QJson::Parser parser;
    QVariantMap result = parser.parse(reply).toMap();

    if(result.contains("error"))
    {
        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value("data").toList();

    QList<SocialItem *> feedItems;

    foreach(QVariant item, list)
    {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem(map);
        feedItems.append(feedItem);
    }
    emit newSocialItems(feedItems);
}
