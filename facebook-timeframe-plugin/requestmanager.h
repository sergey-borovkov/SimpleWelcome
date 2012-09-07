#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <socialplugin.h>

class OAuth2Authorizer;
class FeedItem;
class QNetworkAccessManager;

class RequestManager : public QObject, public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    virtual Request *queryWall(const QDate &beginDate, const QDate &endDate);
    virtual Request *queryImage(const QString &id);
    virtual Request *postComment(const QString &message, const QString &parentId);
    virtual Request *like(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    virtual Request *logout();

private slots:
    void feedReply(QByteArray feedReply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);

private:
    OAuth2Authorizer *m_authorizer;
};

#endif // REQUESTMANAGER_H
