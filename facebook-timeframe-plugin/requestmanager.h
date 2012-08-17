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
    virtual void queryWall(const QDate &beginDate, const QDate &endDate);
    virtual void queryImage(const QString &id);
    virtual void postComment(const QString &postId, const QString &message);
    virtual void like(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    void logout();

private slots:
    void feedReply(QByteArray feedReply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);

private:
    OAuth2Authorizer *m_authorizer;
};

#endif // REQUESTMANAGER_H
