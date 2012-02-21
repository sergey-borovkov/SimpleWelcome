#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

class NepomukSource : public ActivitySource
{
    Q_OBJECT
public:
    explicit NepomukSource(QObject *parent = 0);
    ActivitySet *getActivitySet(int limit, const QDate &beginDate, const QDate &endDate);
signals:
    
public slots:
    
};

#endif // NEPOMUKSOURCE_H
