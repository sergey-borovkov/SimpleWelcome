#ifndef ACTIVITYEVENT_H
#define ACTIVITYEVENT_H

#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>

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
    void setDate(const QDate &d) {
        date = d;
    }

signals:
    void countChanged();

public slots:
    int count();
    Activity *activity(int index);
    QString getUrl(int i);
    QString getType(int i);
    QDate getDate();

private:
    QList<Activity *> activities;
    QDate date;
};

Q_DECLARE_METATYPE(ActivitySet *)
Q_DECLARE_METATYPE(Activity *)

#endif // ACTIVITYEVENT_H
