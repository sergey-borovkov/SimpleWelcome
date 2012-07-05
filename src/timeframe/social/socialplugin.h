#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtPlugin>

class SocialItem;

class ISocialRequestManager : public QObject
{
    Q_OBJECT
public:
    virtual ~ISocialRequestManager() = 0;

public slots:
    virtual void queryWall(const QDate &beginDate, const QDate &endDate) = 0;

signals:
    void authorizationComplete();
    void newSocialItems(QList<SocialItem *> items);
};

class ISocialModule : public QObject
{
    Q_OBJECT
public:
    enum AuthorizationStatus
    {
        Success,
        Expired,
        Failure
    };

    virtual ~ISocialModule() = 0;
    virtual QWidget *authenticationWidget() = 0;
    virtual ISocialRequestManager *requestManager() = 0;

signals:
    void authorizationStatusChanged(int status);
};

Q_DECLARE_INTERFACE(ISocialModule, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
