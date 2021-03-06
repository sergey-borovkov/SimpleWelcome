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

#include "vkontaktemodule.h"
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

VkontakteModule::VkontakteModule()
    : m_authorizer(new OAuth2Authorizer())
    , m_requestManager(new RequestManager())
{
    QSettings settings("ROSA", "vkontakte-timeframe-plugin");
    QString accessToken = settings.value("accessToken").toString();

    m_requestManager->setAuthorizer(m_authorizer);

    connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SLOT(onAcessTokenChanged()));
    connect(m_authorizer, SIGNAL(authorized()), SIGNAL(authorized()));
    connect(m_authorizer, SIGNAL(deauthorized()), SIGNAL(deauthorized()));

    if (!accessToken.isEmpty())
        m_authorizer->setAccessToken(accessToken);

    m_pixmap.load(":/images/vk-shadow.png");
    m_smallPixmap.load(":/images/vk-small.png");
}

VkontakteModule::~VkontakteModule()
{
    delete m_requestManager;
    delete m_authorizer;
}

ISocialRequestManager *VkontakteModule::requestManager()
{
    return m_requestManager;
}

QString VkontakteModule::name() const
{
    return QLatin1String("VKontakte");
}

QPixmap VkontakteModule::icon() const
{
    return m_pixmap;
}

QPixmap VkontakteModule::smallIcon() const
{
    return m_smallPixmap;
}

QString VkontakteModule::displayName() const
{
    return i18n("VKontakte");
}

int VkontakteModule::maximumRequestsPerSecond() const
{
    // VK limits maximum amount of requests per second to 3
    return 3;
}

QWidget *VkontakteModule::authenticationWidget()
{
    QWebView *authorizationView = new QWebView();
    authorizationView->setAttribute(Qt::WA_DeleteOnClose);
    authorizationView->window()->setWindowTitle(name());
    authorizationView->setWindowModality(Qt::ApplicationModal);
    authorizationView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    authorizationView->window()->setWindowIcon(QPixmap(":/images/vk.png"));
    authorizationView->setUrl(QUrl("http://oauth.vk.com/authorize?client_id=2944872&"
                                   "scope=wall,friends,audio,video,offline&"
                                   "redirect_uri=http://oauth.vk.com/blank.html&"
                                   "display=popup&"
                                   "response_type=token"));
    authorizationView->resize(800, 700);
    authorizationView->move(QApplication::desktop()->screen()->rect().center() - authorizationView->rect().center());

    connect(authorizationView, SIGNAL(urlChanged(QUrl)), m_authorizer, SLOT(urlChanged(QUrl)));
    connect(this, SIGNAL(authorized()), authorizationView, SLOT(close()));

    return authorizationView;
}

QString VkontakteModule::selfId() const
{
    return m_selfId;
}

void VkontakteModule::setSelfId(const QString &id)
{
    m_selfId = id;
}

QString VkontakteModule::selfName() const
{
    return m_selfName;
}

void VkontakteModule::setSelfName(const QString &name)
{
    m_selfName = name;
}

QString VkontakteModule::selfPictureUrl() const
{
    if (m_selfPictureUrl.isEmpty()) {
        return QString("images/user.png");
    }
    return m_selfPictureUrl;
}

void VkontakteModule::setSelfPictureUrl(const QString &url)
{
    m_selfPictureUrl = url;
}

void VkontakteModule::setRequestQueue(RequestQueue *requestQueue)
{
    m_requestQueue = requestQueue;
    m_requestQueue->setMaximumRequestsPerSecond(maximumRequestsPerSecond());
    m_requestManager->setRequestQueue(m_requestQueue);
}

RequestQueue *VkontakteModule::requestQueue()
{
    return m_requestQueue;
}

void VkontakteModule::onAcessTokenChanged()
{
}

Q_EXPORT_PLUGIN2(vkontakte-timeframe-plugin, VkontakteModule)





