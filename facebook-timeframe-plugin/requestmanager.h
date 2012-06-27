#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "../src/timeframe/social/socialplugin.h"

class OAuth2Authorizer;

class RequestManager : public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    virtual void queryWall(const QDate &beginDate, const QDate &endDate);
    void setAuthorizer(OAuth2Authorizer *authorizer);

private slots:
    void reply(QByteArray reply);

private:
    OAuth2Authorizer *m_authorizer;
};

#endif // REQUESTMANAGER_H
