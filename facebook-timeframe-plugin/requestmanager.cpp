#include "request.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <qjson/parser.h>

RequestManager::RequestManager(QObject *parent)
    : m_authorizer(0)
{
}

void RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    if(!m_authorizer)
        return;

    Request *request = new Request(m_authorizer->accessToken(), Request::WallPosts, this);
    connect(request, SIGNAL(replyReady(QByteArray)), SLOT(reply(QByteArray)));
    request->startQuery();
}

void RequestManager::setAuthorizer(OAuth2Authorizer *authorizer)
{
    m_authorizer = authorizer;

    if(m_authorizer->isAuthenticated())
        emit authorizationComplete();
    else
        connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SIGNAL(authorizationComplete()));
}

void RequestManager::reply(QByteArray reply)
{
    qDebug() << "reply has come";
}
