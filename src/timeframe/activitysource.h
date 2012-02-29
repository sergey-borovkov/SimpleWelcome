#ifndef ACTIVITYSOURCE_H
#define ACTIVITYSOURCE_H

#include <QObject>

class ActivitySet;
class QDate;
class Activity;

class ActivitySource : public QObject
{
    Q_OBJECT
public:
    explicit ActivitySource(QObject *parent = 0);
    // this function is thread safe
    virtual ActivitySet *getActivitySet(int limit, const QDate &beginDate, const QDate &endDate) = 0;
public slots:
    virtual void startSearch(const QDate &beginDate) = 0;

signals:
    void newActivities(QList<Activity *>);
    void finishedListing();

public slots:
    
};

#endif // ACTIVITYSOURCE_H
