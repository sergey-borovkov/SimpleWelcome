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

#include "facebookmodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <requestqueue.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>

#include <KLocalizedString>

FacebookModule::FacebookModule()
    : m_requestQueue(0)
    , m_authorizer(new OAuth2Authorizer())
    , m_requestManager(new RequestManager())
{
    QSettings settings("ROSA", "facebook-timeframe-plugin");
    QString accessToken = settings.value("accessToken").toString();

    m_requestManager->setAuthorizer(m_authorizer);

    connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SLOT(onAcessTokenChanged()));
    connect(m_authorizer, SIGNAL(authorized()), SIGNAL(authorized()));
    connect(m_authorizer, SIGNAL(deauthorized()), SIGNAL(deauthorized()));

    if (!accessToken.isEmpty())
        m_authorizer->setAccessToken(accessToken);

    m_pixmap.load(":/images/fb-shadow.png");
    m_smallPixmap.load(":/images/fb-small.png");
}

FacebookModule::~FacebookModule()
{
    delete m_requestManager;
    delete m_authorizer;
}

ISocialRequestManager *FacebookModule::requestManager()
{
    return m_requestManager;
}

QString FacebookModule::name() const
{
    return QLatin1String("Facebook");
}

QPixmap FacebookModule::icon() const
{
    return m_pixmap;
}

QPixmap FacebookModule::smallIcon() const
{
    return m_smallPixmap;
}

QString FacebookModule::displayName() const
{
    return i18n("Facebook");
}

int FacebookModule::maximumRequestsPerSecond() const
{
    // google says facebook does not have limit per second
    // but limit like 600 queries per 600 seconds, so there is no meaningful way to set this limit
    // 10 is just an arbitrary number that seems sufficient
    return 10;
}

QWidget *FacebookModule::authenticationWidget()
{
    QWebView *authorizationView = new QWebView();
    authorizationView->setAttribute(Qt::WA_DeleteOnClose);
    authorizationView->setWindowModality(Qt::ApplicationModal);
    authorizationView->window()->setWindowTitle(name());
    authorizationView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    authorizationView->window()->setWindowIcon(QPixmap(":/images/fb.png"));
    authorizationView->setUrl(QUrl("https://www.facebook.com/dialog/oauth?client_id=148453655273563&redirect_uri=http://www.facebook.com/connect/login_success.html&response_type=token&scope=publish_stream,read_stream"));
    authorizationView->resize(1024, 640);
    authorizationView->move(QApplication::desktop()->screen()->rect().center() - authorizationView->rect().center());

    connect(authorizationView, SIGNAL(urlChanged(QUrl)), m_authorizer, SLOT(urlChanged(QUrl)));
    connect(this, SIGNAL(authorized()), authorizationView, SLOT(close()));

    return authorizationView;
}

QString FacebookModule::selfId() const
{
    return m_selfId;
}

void FacebookModule::setSelfId(const QString &id)
{
    m_selfId = id;
}

QString FacebookModule::selfName() const
{
    return m_selfName;
}

void FacebookModule::setSelfName(const QString &name)
{
    m_selfName = name;
}

QString FacebookModule::selfPictureUrl() const
{
    if (m_selfPictureUrl.isEmpty()) {
        return QString("images/user.png");
    }
    return m_selfPictureUrl;
}

void FacebookModule::setSelfPictureUrl(const QString &url)
{
    m_selfPictureUrl = url;
}

void FacebookModule::setRequestQueue(RequestQueue *requestQueue)
{
    m_requestQueue = requestQueue;
    m_requestQueue->setMaximumRequestsPerSecond(maximumRequestsPerSecond());
    m_requestManager->setRequestQueue(requestQueue);
}

RequestQueue *FacebookModule::requestQueue()
{
    return m_requestQueue;
}

void FacebookModule::onAcessTokenChanged()
{
    /*    if(m_authorizer->isAuthorized())
            m_authorizationView->hide(); */
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
