#include "facebookmodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QWebView>


FacebookModule::FacebookModule()
{
    m_authorizer = new OAuth2Authorizer;

    QSettings settings("ROSA", "facebook-timeframe-plugin");
    QString accessToken = settings.value("accessToken").toString();

    if(!accessToken.isEmpty())
    {
        m_authorizer->setAccessToken(accessToken);
        emit authorized();
    }

    m_authorizationView = new QWebView;
    m_authorizationView->resize(800, 700);

    m_requestManager = new RequestManager;
    m_requestManager->setAuthorizer(m_authorizer);

    connect(m_authorizer, SIGNAL(accessTokenChanged(QString)), SLOT(onAcessTokenChanged()));
    connect(m_authorizer, SIGNAL(deauthorized()), SIGNAL(deauthorized()));

    m_pixmap.load(":/images/facebook.png");
}

FacebookModule::~FacebookModule()
{
    delete m_requestManager;
    delete m_authorizer;
    m_authorizationView->deleteLater();
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
    m_authorizationView->setUrl(QUrl("https://www.facebook.com/dialog/oauth?client_id=148453655273563&redirect_uri=http://www.facebook.com/connect/login_success.html&response_type=token&scope=publish_stream,read_stream"));
    connect(m_authorizationView, SIGNAL(urlChanged(QUrl)),
            m_authorizer, SLOT(urlChanged(QUrl)));

    return m_authorizationView;
}

void FacebookModule::onAcessTokenChanged()
{
    if(m_authorizer->isAuthorized())
    {
        m_authorizationView->hide();
        emit authorized();
    }
}

Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
