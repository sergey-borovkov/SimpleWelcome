#include "facebookmodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"
#include <QLabel>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeView>

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
    connect(m_authorizer, SIGNAL(deauthorized()), SIGNAL(deauthorized()));

    m_pixmap.load("/home/user/lang/SimpleWelcome/facebook-timeframe-plugin/images/facebook.png");
    //m_pixmap.load(":/images/facebook.png");
}

FacebookModule::~FacebookModule()
{
    delete m_requestManager;
    delete m_authorizer;
    delete m_authorizationView;
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

QWidget *FacebookModule::authenticationWidget()
{
    return m_authorizationView;
}

void FacebookModule::onAcessTokenChanged()
{
    if(m_authorizer->isAuthorized())
        emit authorized();
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
