#include "vkontaktemodule.h"
#include "requestmanager.h"
#include "oauth2authorizer.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>

#include <KLocalizedString>

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

void VkontakteModule::onAcessTokenChanged()
{
}

Q_EXPORT_PLUGIN2(vkontakte-timeframe-plugin, VkontakteModule)





