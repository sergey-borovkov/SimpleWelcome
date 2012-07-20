#include "oauth2authorizer.h"

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>

const QString OAuth2Authorizer::redirectUrl = "http://oauth.vk.com/blank.html";

OAuth2Authorizer::OAuth2Authorizer( QObject *parent ) :
    QObject( parent )
{
}

void OAuth2Authorizer::setAccessToken( const QString &accessToken )
{
    if ( accessToken != m_accessToken ) {
        m_accessToken = accessToken;

        QSettings settings( "ROSA", "vkontakte-timeframe-plugin" );
        settings.setValue( "accessToken", accessToken );
        qDebug() << "OAuth2Authorizer::setAccessToken:    emit accessTokenChanged!!!!!";
        emit accessTokenChanged( m_accessToken );
    }
}

QString OAuth2Authorizer::accessToken() const
{
    return m_accessToken;
}

bool OAuth2Authorizer::isAuthorized() const
{
    return !accessToken().isEmpty();
}

void OAuth2Authorizer::urlChanged( const QUrl &url )
{
    qDebug() << "OAuth2Authorizer::urlChanged:   url = " << url;
    if ( !url.isEmpty() && url.toString().startsWith( redirectUrl ) ) {
        QString accessToken = url.encodedFragment();        // Get the URL fragment part
        accessToken = accessToken.split( "&" ).first();       // Remove the "expires_in" part.
        accessToken = accessToken.split( "=" ).at( 1 );         // Split by "access_token=..." and take latter part

        qDebug() << "OAuth2Authorizer::urlChanged:   setAccessToken = " << accessToken;
        setAccessToken( accessToken );
    }
}

void OAuth2Authorizer::logout()
{
    qDebug() << "logout...";
    setAccessToken( "" );
    emit deauthorized();
}
