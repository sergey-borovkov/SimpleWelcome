#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QDate>
#include <QtPlugin>

class IWallPost;

class ISocialRequestManager : public QObject
{
    Q_OBJECT
public slots:
    virtual bool getWall(const QDate &beginDate, const QDate &endDate) = 0;
signals:
    virtual void newWallElement(QSharedPointer<IWallPost> post) = 0;
};

class ISocialActivity
{

};

class IWallPost : public ISocialActivity
{

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
Q_DECLARE_INTERFACE(ISocialModule, "Timeframe_Library/1.0")
QT_END_NAMESPACE

#endif // SOCIALPLUGIN_H
