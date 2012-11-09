#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <socialplugin.h>

#include <QtCore/QUrl>
#include <QtCore/QMap>

class OAuth2Authorizer;
class FeedItem;
class CommentItem;
class QNetworkAccessManager;

// response http://oauth.vk.com/oauth/authorize?&_hash=0&success=1
const QString apiVkUrl = QLatin1String("https://api.vk.com/method/");

class RequestManager : public QObject, public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    virtual Request *queryWall(const QDate &beginDate, const QDate &endDate);
    virtual Request *queryUserId();
    virtual Request *queryImage(const QString &id);
    virtual Request *queryAudio(const QString &aid, const QString &ownerId);
    virtual Request *queryVideo(const QString &vid, const QString &ownerId);
    virtual Request *queryComments(const QString &postId);
    virtual Request *queryLikes(const QString &postId);
    virtual Request *postComment(const QByteArray &message, const QString &postId);
    virtual Request *like(const QString &id);
    virtual Request *unlike(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    virtual Request *logout();

private slots:
    void feedReply(QByteArray feedReply);
    void replyQueryWall(QByteArray reply);
    void idReply(QByteArray reply);
    void commentReply(QByteArray reply);
    void likesReply(QByteArray reply);
    void imageReply(QByteArray reply);
    void audioReply(QByteArray reply);
    void videoReply(QByteArray reply);
    void postCommentReply(QByteArray reply);

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);
    void selfId(QString id);
    void selfName(QString name);
    void selfLiked(QString postId);
    void newComments(QString postId, QList<CommentItem *> items);
    void gotUserImage(QString postId, QString url);
    void gotUserName(QString postId, QString name);
    void gotAudioUrl(QString aid, QString ownerId, QString audioUrl);
    void gotVideoUrl(QString vid, QString ownerId, QString videoUrl, QString videoImage);
    void searchComplete();

private:
    QUrl constructUrl(const QString &id) const;
    bool canBeDisplayed(const FeedItem &feedItem) const;

    OAuth2Authorizer *m_authorizer;

    QString m_selfId;
    QString m_selfName;

    // we return comments only when they are fully downloaded
    // so we need to keed partial results between calls somewhere
    QMap<QString, QList<CommentItem *> >m_comments;
    int m_gotCommentsCount;
    int m_gotLikesCount;
    int m_gotMessagesCount;

    const int m_requestItemCount;

};

#endif // REQUESTMANAGER_H
