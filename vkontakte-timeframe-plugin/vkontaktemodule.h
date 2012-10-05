#ifndef FACEBOOK_MODULE_H
#define FACEBOOK_MODULE_H

#include <QtGui/QPixmap>
#include <socialplugin.h>
#include "oauth2authorizer.h"

class QWebView;
class RequestManager;
class OAuth2Authorizer;

class VkontakteModule : public QObject, public ISocialPlugin
{
    Q_OBJECT
    Q_INTERFACES(ISocialPlugin)
public:
    VkontakteModule();
    ~VkontakteModule();

    virtual ISocialRequestManager* requestManager();
    QString name() const;
    QPixmap icon() const;
    QPixmap smallIcon() const;

    QWidget *authenticationWidget();
    bool authorized() const {
        return !m_authorizer->accessToken().isEmpty();
    }

private slots:
    void onAcessTokenChanged();

signals:
    void authorized();
    void deauthorized();

private:
    QWebView *m_authorizationView;
    RequestManager *m_requestManager;
    OAuth2Authorizer *m_authorizer;
    QPixmap m_pixmap;
    QPixmap m_smallPixmap;

};

#endif
