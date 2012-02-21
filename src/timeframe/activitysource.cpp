#include "activitysource.h"
#include "activityset.h"

#include <QDate>


ActivitySource::ActivitySource(QObject *parent) :
    QObject(parent)
{
}

ActivitySet *ActivitySource::getActivitySet(int limit, const QDate &beginDate, const QDate &endDate)
{
    return 0;
}
