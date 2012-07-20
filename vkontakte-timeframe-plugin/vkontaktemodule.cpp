#include "vkontaktemodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QWebView>


VkontakteModule::VkontakteModule()
{
    m_authorizer = new OAuth2Authorizer;

    QSettings settings( "ROSA", "vkontakte-timeframe-plugin" );
    QString accessToken = settings.value( "accessToken" ).toString();

    if ( !accessToken.isEmpty() ) {
        m_authorizer->setAccessToken( accessToken );
        qDebug() << "VkontakteModule::VkontakteModule:  emit authorized!!!!!!!!";
        emit authorized();
    }

    m_authorizationView = new QWebView;
    m_authorizationView->resize(800, 700);

    m_requestManager = new RequestManager;
    m_requestManager->setAuthorizer(m_authorizer);

    connect( m_authorizer, SIGNAL( accessTokenChanged( QString ) ), SLOT( onAcessTokenChanged() ) );
    connect( m_authorizer, SIGNAL( deauthorized() ), SIGNAL( deauthorized() ) );

    m_pixmap.load( ":/images/vkontakte.png" );
}

VkontakteModule::~VkontakteModule()
{
    delete m_requestManager;
    delete m_authorizer;
    m_authorizationView->deleteLater();
}

ISocialRequestManager *VkontakteModule::requestManager()
{
    return m_requestManager;
}

QString VkontakteModule::name() const
{
    return QLatin1String( "VKontakte" );
}

QPixmap VkontakteModule::icon() const
{
    return m_pixmap;
}

QWidget *VkontakteModule::authenticationWidget()
{
    if ( !m_authorizer->isAuthorized() ) {
        m_authorizationView->setUrl( QUrl( "http://oauth.vk.com/authorize?client_id=2944872&"
                                           "scope=wall&"
                                           "redirect_uri=http://oauth.vk.com/blank.html&"
                                           "display=popup&"
                                           "response_type=token" ) );
        connect( m_authorizationView, SIGNAL( urlChanged( QUrl ) ),
                 m_authorizer, SLOT( urlChanged( QUrl ) ) );
    }

    return m_authorizationView;
}

void VkontakteModule::onAcessTokenChanged()
{
    if ( m_authorizer->isAuthorized() ) {
        m_authorizationView->hide();
        qDebug() << "VkontakteModule::onAcessTokenChanged:  emit authorized!!!!!!!!";
        emit authorized();
    }
}

Q_EXPORT_PLUGIN2(vkontakte-timeframe-plugin, VkontakteModule)
