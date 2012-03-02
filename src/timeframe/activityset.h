#ifndef ACTIVITYEVENT_H
#define ACTIVITYEVENT_H

#include <QObject>
#include <QDate>
#include <QList>
#include <QMetaType>

class Activity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
public:
    explicit Activity(const QString &url, const QString &type, const QDate &date, QObject *parent = 0)
        : QObject(parent), url(url), type(type), date(date) {}

signals:
    void urlChanged();
    void typeChanged();

public slots:
    QString getUrl();
    void setUrl(QString newUrl);

    QString getType();
    void setType(QString newType);

    QDate getDate();
    void setDate(QDate date);

private:
    QString url;
    QString type;
    QDate date;
};

// set of activities for day
class ActivitySet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ActivitySet(QObject *parent = 0);
    ActivitySet(QList<Activity *> activities, QObject *parent = 0);
    ~ActivitySet();

    void addActivity(const QString &url, const QString &type, const QDate &date);
    void addActivity(Activity *activity);
signals:
    void countChanged();

public slots:
    int count();
    Activity *activity(int index);    
    QString getUrl(int i);
    QString getType(int i);
    QDate getDate(int i);

private:
    QList<Activity *> activities;    
    QDate date;
};

Q_DECLARE_METATYPE(ActivitySet *)

#endif // ACTIVITYEVENT_H
