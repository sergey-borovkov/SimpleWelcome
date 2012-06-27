#include "facebookmodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

FacebookModule::FacebookModule()
{
    m_authorizationView = new QDeclarativeView(QUrl("qrc:/qml/main.qml"));

    connect(m_authorizationView->engine(), SIGNAL(quit()), m_authorizationView, SLOT(close()));

    m_authorizer = new OAuth2Authorizer;
    connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SLOT(onAcessTokenChanged()));

    m_authorizationView->engine()->rootContext()->setContextProperty("authorizer", m_authorizer);

    m_requestManager = new RequestManager;
}

ISocialRequestManager *FacebookModule::requestManager()
{
    return m_requestManager;
}

QWidget *FacebookModule::authenticationWidget()
{
    return m_authorizationView;
}

void FacebookModule::onAcessTokenChanged()
{
    emit authorizationStatusChanged(ISocialModule::Success);
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
