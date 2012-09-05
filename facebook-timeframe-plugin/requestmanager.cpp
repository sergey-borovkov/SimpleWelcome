#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QStringList>

RequestManager::RequestManager(QObject *parent)
    : m_authorizer(0)
{
}

Request *RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    Q_UNUSED(beginDate)
    Q_UNUSED(endDate)

    QUrl url(QLatin1String("https://graph.facebook.com/me/feed"));
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());

    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(feedReply(QByteArray)));

    request->setUrl(url);
    return request;
}

Request *RequestManager::queryImage(const QString &id)
{
    Q_UNUSED(id)
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Get, this);
    return request;
}

Request *RequestManager::postComment(const QString &parent, const QString &message)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Post, this);
    QUrl url = QLatin1String("https://graph.facebook.com/") + parent + QLatin1String("/comments");
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());
    url.addQueryItem("message", message);
    return request;
}

Request *RequestManager::like(const QString &id)
{
    FacebookRequest *request = new FacebookRequest(FacebookRequest::Post, this);
    QUrl url = QLatin1String("https://graph.facebook.com/") + id + QLatin1String("/likes");
    url.addQueryItem(QLatin1String("access_token"), m_authorizer->accessToken());
    request->setUrl(url);
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

    if(result.contains(QLatin1String("error"))) {
        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value(QLatin1String("data")).toList();
    QList<SocialItem *> feedItems;

    foreach(QVariant item, list) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem(map);
        feedItems.append(feedItem);
    }
    emit newSocialItems(feedItems);
}
