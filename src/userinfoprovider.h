#pragma once

#include <QObject>
#include <KUser>

class UserInfoProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullName READ getFullName)
    Q_PROPERTY(QString loginName READ getLoginName)
    Q_PROPERTY(QString userName READ getUserName)

public:
    UserInfoProvider(QObject *parent = 0);

    QString getFullName();
    QString getLoginName();
    QString getUserName();
    QString getIconPath();

private:
    KUser m_userInfo;
};
