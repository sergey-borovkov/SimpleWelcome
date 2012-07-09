#include "oauth2authorizer.h"

#include <QtCore/QSettings>
#include <QtCore/QtDebug>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

const QString OAuth2Authorizer::redirectUrl = "http://www.facebook.com/connect/login_success.html";

OAuth2Authorizer::OAuth2Authorizer(QObject *parent) :
    QObject(parent)
{
    //setAccessToken("AAACHBI9jYFsBAKknxU3jOa0xthTuJBCqZCdD8ogzOtyGpHhlyBglpjEiRuWEhajx6cV0dZBd2bGe6ZC3Xe2Ystma14wlmA4YmQcSjitlQZDZD");
}

void OAuth2Authorizer::setAccessToken(const QString &accessToken)
{
    if (accessToken != m_accessToken)
    {
        qDebug() << "Set access token as: " << accessToken;
        m_accessToken = accessToken;
        emit accessTokenChanged(m_accessToken);
    }
}

bool OAuth2Authorizer::isAuthenticated() const
{
    return !accessToken().isEmpty();
}

QString OAuth2Authorizer::accessToken() const
{
    return m_accessToken;
}

void OAuth2Authorizer::urlChanged(const QUrl &url)
{
    qDebug() << "Web loading started: " << url;
    if (!url.isEmpty() && url.toString().startsWith(redirectUrl))
    {
        QString accessToken = url.encodedFragment();        // Get the URL fragment part
        accessToken = accessToken.split("&").first();       // Remove the "expires_in" part.
        accessToken = accessToken.split("=").at(1);         // Split by "access_token=..." and take latter part
        QSettings settings("ROSA", "facebook-timeframe-plugin");
        settings.setValue("accessToken", accessToken);
        setAccessToken(accessToken);
    }
}
