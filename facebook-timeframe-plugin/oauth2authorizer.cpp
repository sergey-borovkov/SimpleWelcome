#include "oauth2authorizer.h"

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include <QtCore/QUrl>

const QString OAuth2Authorizer::redirectUrl = "http://www.facebook.com/connect/login_success.html";

OAuth2Authorizer::OAuth2Authorizer(QObject *parent) :
    QObject(parent)
{
}

void OAuth2Authorizer::setAccessToken(const QString &accessToken)
{
    if (accessToken != m_accessToken)
    {
        m_accessToken = accessToken;

        QSettings settings("ROSA", "facebook-timeframe-plugin");
        settings.setValue("accessToken", accessToken);
        emit accessTokenChanged(m_accessToken);
    }
}

void OAuth2Authorizer::logout()
{
    qDebug() << "logout...";
    if(!accessToken().isEmpty())
    {
        setAccessToken("");
        emit deauthorized();
    }
}

bool OAuth2Authorizer::isAuthorized() const
{
    return !accessToken().isEmpty();
}

QString OAuth2Authorizer::accessToken() const
{
    return m_accessToken;
}

void OAuth2Authorizer::urlChanged(const QUrl &url)
{
    if (!url.isEmpty() && url.toString().startsWith(redirectUrl))
    {
        QString accessToken = url.encodedFragment();        // Get the URL fragment part
        accessToken = accessToken.split("&").first();       // Remove the "expires_in" part.
        accessToken = accessToken.split("=").at(1);         // Split by "access_token=..." and take latter part
        setAccessToken(accessToken);
    }
}
