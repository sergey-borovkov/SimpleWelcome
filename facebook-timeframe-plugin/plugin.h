#ifndef PLUGIN1_H
#define PLUGIN1_H

#include "../src/timeframe/social/socialplugin.h"

class QDeclarativeView;
class RequestManager;
class OAuth2Authorizer;

class FacebookModule : public ISocialModule
{
    Q_OBJECT
    Q_INTERFACES(ISocialModule)
public:
    FacebookModule();

    virtual ISocialRequestManager* requestManager();
    QWidget *authenticationWidget();

private slots:
    void onAcessTokenChanged();
private:
    QDeclarativeView *m_authorizationView;
    RequestManager *m_requestManager;
    OAuth2Authorizer *m_authorizer;
};

#endif
