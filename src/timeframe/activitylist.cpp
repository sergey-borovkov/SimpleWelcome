#include "activitylist.h"
#include "activityset.h"

ActivityList::ActivityList( int year, int month, QObject *parent) :
    QObject(parent), d(QDate(year, month, 1))
{    
}

int ActivityList::count()
{
    return list.size();
}

void ActivityList::addSet(ActivitySet *set)
{
    list.append(set);
}

ActivitySet *ActivityList::at(int i) const
{
    return list.at(i);
}
