#include "plugin.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

FacebookModule::FacebookModule()
{
    m_authorizationView = new QDeclarativeView(QUrl("qrc:/qml/main.qml"));
    m_requestManager = new RequestManager;
    m_authorizer = new OAuth2Authorizer;

    m_authorizationView->engine()->rootContext()->setContextProperty("authorizer", m_authorizer);
}

ISocialRequestManager *FacebookModule::requestManager()
{
    return m_requestManager;
}

QWidget *FacebookModule::authenticationWidget()
{
    return m_authorizationView;
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
