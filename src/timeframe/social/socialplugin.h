#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtGui/QPixmap>
#include <QtPlugin>

class SocialItem;

class ISocialRequestManager
{
public:
    virtual ~ISocialRequestManager() {}
    virtual void queryWall(const QDate &beginDate, const QDate &endDate) = 0;
};

class ISocialPlugin
{
public:
    enum AuthorizationStatus
    {
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
