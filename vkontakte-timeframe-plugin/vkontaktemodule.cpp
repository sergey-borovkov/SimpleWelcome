#include "vkontaktemodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>


VkontakteModule::VkontakteModule()
{
    m_authorizer = new OAuth2Authorizer;

    QSettings settings("ROSA", "vkontakte-timeframe-plugin");
    QString accessToken = settings.value("accessToken").toString();

    m_requestManager = new RequestManager;
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
    m_authorizationView->deleteLater();
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

QWidget *VkontakteModule::authenticationWidget()
{
    m_authorizationView = new QWebView;
    m_authorizationView->window()->setWindowTitle(name());
    m_authorizationView->setWindowModality(Qt::ApplicationModal);
    m_authorizationView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_authorizationView->window()->setWindowIcon(QPixmap(":/images/vk.png"));
    m_authorizationView->resize(800, 700);
    m_authorizationView->setUrl(QUrl("http://oauth.vk.com/authorize?client_id=2944872&"
                                     "scope=wall,friends&"
                                     "redirect_uri=http://oauth.vk.com/blank.html&"
                                     "display=popup&"
                                     "response_type=token"));
    connect(m_authorizationView, SIGNAL(urlChanged(QUrl)), m_authorizer, SLOT(urlChanged(QUrl)));
    connect(this, SIGNAL(authorized()), m_authorizationView, SLOT(hide()));

    return m_authorizationView;
}

void VkontakteModule::onAcessTokenChanged()
{
}

Q_EXPORT_PLUGIN2(vkontakte-timeframe-plugin, VkontakteModule)





