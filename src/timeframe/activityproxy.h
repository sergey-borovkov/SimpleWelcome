#ifndef ACTIVITYPROXY_H
#define ACTIVITYPROXY_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QMetaType>

#include "activitysource.h"

class ActivitySet;
class ActivityList;

class ActivityProxy : public QObject
{
    Q_OBJECT
public:
    explicit ActivityProxy(QObject *parent = 0);
    ~ActivityProxy();

    void addSource(ActivitySource *source);

signals:
    void newSearch(QDate, ActivitySource::Direction);
    void newActivitySet(ActivitySet*);
    void newMonth(int year, int month);
    void monthFinished(QDate date);
    void finished();

public slots:
    void addActivitySet(ActivitySet *set);
    void setMonth(int year, int month);
    void startNewSearch(int year, int month, bool direction);


private slots:
    void listingFinished();

private:

    QList < ActivityList *> activityList;
};

#endif // ACTIVITYPROXY_H
