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
    virtual Request *queryUserId();
    virtual Request *queryImage(const QString &id);
    virtual Request *queryComments(const QString &postId);
    virtual Request *postComment(const QString &message, const QString &parentId);
    virtual Request *like(const QString &id);
    virtual Request *dislike(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    virtual Request *logout();


private slots:
    void feedReply(QByteArray feedReply);
    void idReply(QByteArray reply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);
    void userId(QString id);

private:
    OAuth2Authorizer *m_authorizer;
    QString m_selfId;
};

#endif // REQUESTMANAGER_H
