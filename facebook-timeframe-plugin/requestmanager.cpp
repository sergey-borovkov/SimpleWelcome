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
    Q_UNUSED(beginDate)
    Q_UNUSED(endDate)

    if(!m_authorizer)
        return;

    QUrl url(QLatin1String("https://graph.facebook.com/me/feed"));
    url.addQueryItem("access_token", m_authorizer->accessToken());

    Request *request = new Request(Request::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(reply(QByteArray)));

    request->setUrl(url);
    request->startQuery();
}

void RequestManager::queryImage(const QString &id)
{
    Q_UNUSED(id)
}

void RequestManager::postComment(const QString &parent)
{
    //Request *request = new Request(m_authorizer->accessToken(), Request::PostComment, this);
    //request->setQ
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
    Request *request = new Request(Request::Get, this);
    connect(request, SIGNAL(success()), m_authorizer, SLOT(logout()));

    QUrl url(QLatin1String("https://www.facebook.com/logout.php"));
    url.addQueryItem("access_token", m_authorizer->accessToken());
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
