#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtGui/QPixmap>
#include <QtPlugin>

class SocialItem;

/* All subclasses of this class should emit
   success() and error(QString errorString) signals
*/

class Request {
public:
    virtual ~Request() {}
    virtual void start() = 0;
};

class ISocialRequestManager
{
public:
    virtual ~ISocialRequestManager() {}
    virtual Request *queryWall(const QDate &beginDate, const QDate &endDate) = 0;
    virtual Request *queryImage(const QString &id) = 0;
    virtual Request *postComment(const QString &postId, const QString &message) = 0;
    virtual Request *like(const QString &id) = 0;
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
    virtual QWidget *authenticationWidget() = 0;
    virtual ISocialRequestManager *requestManager() = 0;
    virtual bool authorized() const = 0;
};

Q_DECLARE_INTERFACE(ISocialPlugin, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
