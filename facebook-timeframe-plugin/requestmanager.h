#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <socialplugin.h>

class OAuth2Authorizer;
class FeedItem;

class RequestManager : public QObject, public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    ~RequestManager();
    virtual void queryWall(const QDate &beginDate, const QDate &endDate);
    void setAuthorizer(OAuth2Authorizer *authorizer);

private:
    FeedItem *parseReply(const QByteArray &reply);

private slots:
    void reply(QByteArray reply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);

private:
    OAuth2Authorizer *m_authorizer;
};

#endif // REQUESTMANAGER_H
