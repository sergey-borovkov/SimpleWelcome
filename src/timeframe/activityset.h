#ifndef ACTIVITYEVENT_H
#define ACTIVITYEVENT_H

#include <QObject>
#include <QDate>
#include <QDateTime>
#include <QList>

class Activity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ getUrl WRITE setUrl)
    Q_PROPERTY(QString type READ getType WRITE setType)
public:
    explicit Activity(const QString &url, const QString &type, QObject *parent = 0)
        : QObject(parent), url(url), type(type) {}
public slots:
    QString getUrl();
    void setUrl(QString newUrl);

    QString getType();
    void setType(QString newType);

private:
    QString url;
    QString type;
    QDateTime datetime;
};

// set of activities for some period of time
class ActivitySet : public QObject
{
    Q_OBJECT
public:
    explicit ActivitySet(QObject *parent = 0);
    ~ActivitySet();

    void addActivity(const QString &url, const QString &type);
signals:
    
public slots:
    int count() ;
    Activity *activity(int index);
    QDate beginDate();
    QDate endDate();

private:
    QList<Activity *> activities;
};

#endif // ACTIVITYEVENT_H
