#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <socialplugin.h>

#include <QtCore/QUrl>
#include <QtCore/QMap>

class OAuth2Authorizer;
class FeedItem;
class CommentItem;
class QNetworkAccessManager;

const QString openGraphUrl = QLatin1String("https://graph.facebook.com/");

class RequestManager : public QObject, public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    virtual Request *queryWall(const QDate &beginDate, const QDate &endDate);
    virtual Request *queryUserId();
    virtual Request *queryImage(const QString &id);
    virtual Request *queryComments(const QString &postId);
    virtual Request *queryLikes(const QString &postId);
    virtual Request *postComment(const QString &message, const QString &parentId);
    virtual Request *like(const QString &id);
    virtual Request *unlike(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    virtual Request *logout();

private slots:
    void feedReply(QByteArray feedReply);
    void idReply(QByteArray reply);
    void commentReply(QByteArray reply);
    void imageReply(QByteArray reply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);
    void selfId(QString id);
    void selfName(QString name);
    void selfLiked(QString postId);
    void newComments(QString postId, QList<CommentItem *> items);
    void gotUserImage(QString id, QString url);
    void gotUserName(QString postId, QString name);

private:
    QUrl constructUrl(const QString &id, const QString &type) const;

    OAuth2Authorizer *m_authorizer;
    QString m_selfId;
    QString m_selfName;

    // we return comments only when they are fully downloaded
    // so we need to keed partial results between calls somewhere
    QMap<QString, QList<CommentItem *> >m_comments;
};

#endif // REQUESTMANAGER_H
