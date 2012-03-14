#ifndef ACTIVITYPROXY_H
#define ACTIVITYPROXY_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QMetaType>

class ActivitySet;
class ActivitySource;
class ActivityList;

class ActivityProxy : public QObject
{
    Q_OBJECT
public:
    explicit ActivityProxy(QObject *parent = 0);
    ~ActivityProxy();

    void addSource(ActivitySource *source);

signals:
    void newSearch(QDate);
    void newList(int index, ActivityList *list);
    void listChanged(int index, ActivityList *list);

public slots:
    void addActivitySet(ActivitySet *set);
    void setMonth(int year, int month);

private slots:
    void listingFinished();

private:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole
    };

    QList < ActivityList *> activityList;
};

#endif // ACTIVITYPROXY_H
