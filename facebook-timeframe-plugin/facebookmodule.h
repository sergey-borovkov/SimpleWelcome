#ifndef FACEBOOK_MODULE_H
#define FACEBOOK_MODULE_H

#include <QtGui/QPixmap>
#include <socialplugin.h>
#include "oauth2authorizer.h"

class QWebView;
class RequestManager;
class OAuth2Authorizer;

class FacebookModule : public QObject, public ISocialPlugin
{
    Q_OBJECT
    Q_INTERFACES(ISocialPlugin)
public:
    FacebookModule();
    ~FacebookModule();

    virtual ISocialRequestManager* requestManager();
    QString name() const;
    QPixmap icon() const;

    QWidget *authenticationWidget();
    bool deauthorize();
    bool authorized() const { return !m_authorizer->accessToken().isEmpty(); }

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
};

#endif
