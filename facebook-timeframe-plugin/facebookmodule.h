/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
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

#ifndef FACEBOOK_MODULE_H
#define FACEBOOK_MODULE_H

#include <QtGui/QPixmap>
#include <socialplugin.h>
#include "oauth2authorizer.h"

class QWebView;
class RequestManager;
class OAuth2Authorizer;

class FacebookModule : public QObject, public ISocialPlugin
{
    Q_OBJECT
    Q_INTERFACES(ISocialPlugin)
public:
    FacebookModule();
    ~FacebookModule();

    virtual ISocialRequestManager *requestManager();
    QString name() const;
    QPixmap icon() const;
    QPixmap smallIcon() const;
    QString displayName() const;
    int maximumRequestsPerSecond() const;

    QWidget *authenticationWidget();
    bool authorized() const {
        return !m_authorizer->accessToken().isEmpty();
    }

    QString selfId() const;
    void setSelfId(const QString &id);

    QString selfName() const;
    void setSelfName(const QString &name);

    QString selfPictureUrl() const;
    void setSelfPictureUrl(const QString &url);

    virtual void setRequestQueue(RequestQueue *requestQueue);
    virtual RequestQueue *requestQueue();

private slots:
    void onAcessTokenChanged();

signals:
    void authorized();
    void deauthorized();

private:
    RequestManager *m_requestManager;
    OAuth2Authorizer *m_authorizer;
    QPixmap m_pixmap;
    QPixmap m_smallPixmap;

    // User info
    QString m_selfId;
    QString m_selfName;
    QString m_selfPictureUrl;
    RequestQueue *m_requestQueue;
};

#endif
