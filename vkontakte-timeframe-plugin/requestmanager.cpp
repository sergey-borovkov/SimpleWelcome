#include "request.h"
#include "feeditem.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

RequestManager::RequestManager( QObject *parent )
    : QObject( parent )
    , m_authorizer( 0 )
{
}

void RequestManager::queryWall( const QDate &beginDate, const QDate &endDate )
{
    if ( !m_authorizer )
        return;

    Request *request = new Request( m_authorizer->accessToken(), Request::WallPosts, this, 0 );

    connect( request, SIGNAL( replyReady( QByteArray ) ), SLOT( reply( QByteArray ) ) );
    request->startQuery();
}

void RequestManager::setAuthorizer( OAuth2Authorizer *authorizer )
{
    m_authorizer = authorizer;

    if ( m_authorizer->isAuthorized() )
        emit authorizationComplete();
    else
        connect( m_authorizer, SIGNAL( accessTokenChanged( QString ) ), SIGNAL( authorizationComplete() ) );
}

void RequestManager::logout()
{
    Request *request = new Request( m_authorizer->accessToken(), Request::Logout );
    connect( request, SIGNAL( success() ), m_authorizer, SLOT( logout() ) );
    request->startQuery();

    // actually first need to do some error checking
    m_authorizer->logout();
}

void RequestManager::reply( QByteArray reply )
{
    qDebug() << "RequestManager::reply";
    QJson::Parser parser;
    QVariantMap result = parser.parse( reply ).toMap();

    if ( result.contains( "error" ) ) {
        m_authorizer->logout();
        qDebug() << "RequestManager::reply:   got ERROR from server...";
        return;
    }

    int itemCount = 0;
    QVariantList list = result.value( "response" ).toList();
    if ( list.size() > 1 ) {
        itemCount = list.at( 0 ).toInt();
        // delete first item from list because first item is a count of messages in the wall
        list.takeFirst();
    }

    int cycles = 0;
    if ( itemCount >= 100 ) {
        cycles = itemCount / 100;
    }

    // need repeat query
    if ( cycles ) {
        for ( int i = 0; i < cycles; i++ ) {
            Request *request = new Request( m_authorizer->accessToken(), Request::WallPosts, this, 100 * ( i + 1 ) );
            connect( request, SIGNAL( replyReady( QByteArray ) ), SLOT( replyQueryWall( QByteArray ) ) );
            request->startQuery();
        }
    }

    QList<SocialItem *> feedItems;

    foreach ( QVariant item, list ) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem();
        feedItem->fillFromMap(map);
        feedItems.append( feedItem );
    }
    emit newSocialItems( feedItems );
}


void RequestManager::replyQueryWall( QByteArray reply )
{
    QJson::Parser parser;
    QVariantMap result = parser.parse( reply ).toMap();

    if ( result.contains( "error" ) ) {
        m_authorizer->logout();
        return;
    }

    QVariantList list = result.value( "response" ).toList();
    if ( list.size() > 1 ) {
        list.takeFirst();
    }

    QList<SocialItem *> feedItems;

    foreach ( QVariant item, list ) {
        QVariantMap map = item.toMap();
        FeedItem *feedItem = new FeedItem();
        feedItem->fillFromMap(map);
        feedItems.append( feedItem );
    }

    emit newSocialItems( feedItems );
}
