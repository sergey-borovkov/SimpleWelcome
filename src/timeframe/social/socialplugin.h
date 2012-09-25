#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QList>
#include <QtGui/QPixmap>
#include <QtCore/QtPlugin>

class SocialItem;
class QDate;

/**
 * @brief The Request class encapsulates all write requests to social plugins .
 *        All subclasses of this class in social plugins should emit success()
 *        and error(QString errorString) signals when appropriate.
 */
class Request {
public:
    virtual ~Request() {}
    /**
     * @brief start Start request. This method must be not blocking.
     */
    virtual void start() = 0;
};

class ISocialRequestManager
{
public:
    virtual ~ISocialRequestManager() {}
    virtual Request *queryWall(const QDate &beginDate, const QDate &endDate) = 0;
    virtual Request *queryUserId() = 0;
    virtual Request *queryImage(const QString &id) = 0;
    virtual Request *postComment(const QString &message, const QString &parentId) = 0;

    /**
     * @brief Query comments to post. You need to connect to SocialRequestManager
     *        newComments(QList<CommentItem *>) signal to get result
     * @param id of parent post
     * @return pointer to request
     */
    virtual Request *queryComments(const QString &postId) = 0;
    virtual Request *like(const QString &id) = 0;
    virtual Request *unlike(const QString &id) = 0;
    virtual Request *logout() = 0;
};

class ISocialPlugin
{
public:
    enum AuthorizationStatus {
        Success,
        Expired,
        Failure
    };

    virtual ~ISocialPlugin() {}
    virtual QString name() const = 0;
    virtual QPixmap icon() const = 0;
    virtual QPixmap smallIcon() const = 0;
    virtual QWidget *authenticationWidget() = 0;
    virtual ISocialRequestManager *requestManager() = 0;
    virtual bool authorized() const = 0;
};

Q_DECLARE_INTERFACE(ISocialPlugin, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
