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

#ifndef OAUTH2AUTHORIZER_H
#define OAUTH2AUTHORIZER_H

#include <QtCore/QObject>

class QUrl;

class OAuth2Authorizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
public:
    static const QString redirectUrl;
    static const QString redirectUrl1;

    explicit OAuth2Authorizer(QObject *parent = 0);
    QString accessToken() const;
    void setAccessToken(const QString &accessToken);
    bool isAuthorized() const;

signals:
    void accessTokenChanged(QString accessToken);
    void authorized();
    void deauthorized();

public slots:
    void urlChanged(const QUrl &url);
    void logout();

private:
    QString m_accessToken;
};

#endif // OAUTH2AUTHORIZER_H
