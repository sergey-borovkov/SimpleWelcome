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
    enum Direction
    {
        Right,
        Left
    };

    explicit ActivitySource(QObject *parent = 0);
    virtual ActivitySet *getActivitySet(int limit, const QDate &beginDate, const QDate &endDate) = 0;
public slots:
    virtual void startSearch(const QDate &beginDate, ActivitySource::Direction direction) = 0;
    virtual void setLimit(int limit) = 0;

signals:
    void newActivities(QList<Activity *>);
    void newActivitySet(ActivitySet *);
    void finishedListing();
    void monthFinished(QDate date);
};

#endif // ACTIVITYSOURCE_H
