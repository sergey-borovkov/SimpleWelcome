#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtPlugin>

class ISocialRequestManager : public QObject
{
    Q_OBJECT
public:
    virtual ~ISocialRequestManager() = 0;

public slots:
    virtual void queryWall(const QDate &beginDate, const QDate &endDate) = 0;
};

class User
{
public:    
    QString firstName() const { return m_firstName; }
    void setFirstName(const QString &name) { m_firstName = name; }
    QString lastName() const { return m_lastName; }
    void setLastName(const QString &name) { m_lastName = name; }
    QUrl url() const { return m_url; }
    void setUrl(const QUrl &url) { m_url = url; }
private:
    QString m_firstName;
    QString m_lastName;
    QUrl m_url;
};

class ISocialModule
{
public:
    virtual ~ISocialModule() = 0;
    virtual QWidget *authenticationWidget() = 0;
    ISocialRequestManager *requestManager();
};

Q_DECLARE_INTERFACE(ISocialModule, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
