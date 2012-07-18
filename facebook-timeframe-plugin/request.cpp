#include "request.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

const QString Request::wallUrl = QLatin1String("https://graph.facebook.com/me/feed");
const QString Request::logoutUrl = QLatin1String("https://www.facebook.com/logout.php");

Request::Request(const QString &accessToken, RequestType type, QObject *parent) :
    QObject(parent), m_accessToken(accessToken), m_type(type)
{
}

void Request::startQuery()
{
    QUrl requestUrl;

    if(m_type == WallPosts)
    {
        requestUrl = wallUrl;
        requestUrl.addQueryItem(QLatin1String("access_token"), m_accessToken);
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(requestUrl));
}

void Request::replyFinished(QNetworkReply *reply)
{
    QByteArray a = reply->readAll();
    emit replyReady(a);
    reply->deleteLater();
}

void Request::networkError(QNetworkReply::NetworkError)
{
}
