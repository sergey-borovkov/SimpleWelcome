#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtPlugin>

class ISocialActivity
{

};

class WallPost
{

};

class ISocialRequestManager : public QObject
{
    Q_OBJECT
public slots:
    virtual void queryWall(const QDate &beginDate, const QDate &endDate) = 0;
signals:
    virtual void newWallElement(IWallPost post);
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

class ISocialModule //: public QObject
{
public:
    ISocialModule()
    {}
    virtual ~ISocialModule() = 0;
    virtual QWidget *authenticationWidget() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISocialModule, "Timeframe_Library.SocialModule/1.0")
QT_END_NAMESPACE
#endif // SOCIALPLUGIN_H
