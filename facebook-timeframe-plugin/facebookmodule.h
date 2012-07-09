#ifndef FACEBOOK_MODULE_H
#define FACEBOOK_MODULE_H

#include <socialplugin.h>
#include "oauth2authorizer.h"

class QDeclarativeView;
class RequestManager;
class OAuth2Authorizer;

class FacebookModule : public QObject, public ISocialModule
{
    Q_OBJECT
    Q_INTERFACES(ISocialModule)
public:
    FacebookModule();

    virtual ISocialRequestManager* requestManager();
    QWidget *authenticationWidget();
    bool isAuthorized() const { return !m_authorizer->accessToken().isEmpty(); }

private slots:
    void onAcessTokenChanged();

signals:
    void authorized();

private:
    QDeclarativeView *m_authorizationView;
    RequestManager *m_requestManager;
    OAuth2Authorizer *m_authorizer;
};

#endif
