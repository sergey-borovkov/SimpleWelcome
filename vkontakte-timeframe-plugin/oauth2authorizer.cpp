/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Julia Mineeva <julia.mineeva@osinit.ru>
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "oauth2authorizer.h"

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include <QtCore/QUrl>

const QString OAuth2Authorizer::redirectUrl = "http://oauth.vk.com/blank.html";
const QString OAuth2Authorizer::redirectUrl1 = "https://oauth.vk.com/blank.html";

OAuth2Authorizer::OAuth2Authorizer(QObject *parent) :
    QObject(parent)
{
}

void OAuth2Authorizer::setAccessToken(const QString &accessToken)
{
    if (accessToken != m_accessToken) {
        m_accessToken = accessToken;
        QSettings settings("ROSA", "vkontakte-timeframe-plugin");
        settings.setValue("accessToken", accessToken);
        emit accessTokenChanged(m_accessToken);

        if (!accessToken.isEmpty())
            emit authorized();
    }
}

void OAuth2Authorizer::logout()
{
    if (!accessToken().isEmpty()) {
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
    if (!url.isEmpty() && (url.toString().startsWith(redirectUrl) || url.toString().startsWith(redirectUrl1))) {
        QString accessToken = url.encodedFragment();        // Get the URL fragment part
        accessToken = accessToken.split("&").first();       // Remove the "expires_in" part.
        accessToken = accessToken.split("=").at(1);         // Split by "access_token=..." and take latter part
        setAccessToken(accessToken);
    }
}

