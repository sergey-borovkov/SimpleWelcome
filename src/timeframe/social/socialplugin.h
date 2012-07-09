#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtPlugin>

class SocialItem;

class ISocialRequestManager
{
public:
    virtual ~ISocialRequestManager() {}
    virtual void queryWall(const QDate &beginDate, const QDate &endDate) = 0;
};

class ISocialModule
{
public:
    enum AuthorizationStatus
    {
        Success,
        Expired,
        Failure
    };

    virtual ~ISocialModule() {}
    virtual QWidget *authenticationWidget() = 0;
    virtual ISocialRequestManager *requestManager() = 0;
    virtual bool isAuthorized() const = 0;
};

Q_DECLARE_INTERFACE(ISocialModule, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
