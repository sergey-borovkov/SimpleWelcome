#include "request.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

const QString Request::wallUrl = QLatin1String( "https://api.vk.com/method/wall.get" );
const QString Request::logoutUrl = QLatin1String( "http://oauth.vk.com/oauth/logout" );
// response http://oauth.vk.com/oauth/authorize?&_hash=0&success=1

Request::Request(const QString &accessToken, RequestType type, QObject *parent) :
    QObject(parent), m_accessToken(accessToken), m_type(type)
{
}

void Request::startQuery()
{
    QUrl requestUrl;

    if ( m_type == WallPosts ) {
        requestUrl = wallUrl;
        requestUrl.addQueryItem( QLatin1String( "offset" ), "0" );
        requestUrl.addQueryItem( QLatin1String( "count" ), "100" );
        requestUrl.addQueryItem( QLatin1String( "filter" ), "all" );

        requestUrl.addQueryItem( QLatin1String( "access_token" ), m_accessToken );
    }
    else if ( m_type == Logout ) {
        requestUrl = logoutUrl;
    }
    qDebug() << "Request::startQuery():   requestUrl is " << requestUrl;

    QNetworkAccessManager *manager = new QNetworkAccessManager( this );
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( replyFinished( QNetworkReply* ) ) );
    manager->get( QNetworkRequest( requestUrl) );
}

void Request::replyFinished( QNetworkReply *reply )
{
    QByteArray a = reply->readAll();
    emit replyReady( a );
    reply->deleteLater();
}

void Request::networkError( QNetworkReply::NetworkError )
{
}
