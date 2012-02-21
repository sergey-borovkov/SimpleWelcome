#ifndef ACTIVITYSOURCE_H
#define ACTIVITYSOURCE_H

#include <QObject>


class ActivitySet;
class QDate;

class ActivitySource : public QObject
{
    Q_OBJECT
public:
    explicit ActivitySource(QObject *parent = 0);

    // this function is thread safe
    virtual ActivitySet *getActivitySet(int limit, const QDate &beginDate, const QDate &endDate) = 0;
signals:
    void newActivitySet(ActivitySet *);

public slots:
    
};

#endif // ACTIVITYSOURCE_H
