#include "facebookmodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

FacebookModule::FacebookModule()
{
    m_authorizer = new OAuth2Authorizer;

    QSettings settings("ROSA", "facebook-timeframe-plugin");
    QString accessToken = settings.value("accessToken").toString();


    if(!accessToken.isEmpty())
    {
        m_authorizer->setAccessToken(accessToken);
        m_authorizationView = 0;
        emit authorized();
    }
    else
    {
        m_authorizationView = new QDeclarativeView();
        m_authorizationView->engine()->rootContext()->setContextProperty("authorizer", m_authorizer);
        m_authorizationView->setSource(QUrl("qrc:/qml/main.qml"));
    }

    m_requestManager = new RequestManager;
    m_requestManager->setAuthorizer(m_authorizer);

    connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SLOT(onAcessTokenChanged()));
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
    //emit authorizationStatusChanged(ISocialModule::Success);
    emit authorized();
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)