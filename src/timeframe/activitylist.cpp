#include "activitylist.h"
#include "activityset.h"

#include <QtCore/QDebug>

ActivityList::ActivityList( int year, int month, QObject *parent) :
    QObject(parent), d(QDate(year, month, 1)), m_complete(false)
{    
}

int ActivityList::count() const
{    
    return list.size();
}

void ActivityList::addSet(ActivitySet *set)
{
    int index = 0;

    // find place to insert set
    for(; index < list.count() && list[index]->getDate() < set->getDate(); index++);

    list.insert(index, set);

    emit countChanged();
}

ActivitySet *ActivityList::at(int i) const
{
    return list.at(i);
}

QString ActivityList::getUrl(int set, int index)
{
    return list[set]->getUrl(index);
}

int ActivityList::getSetCount(int index)
{    
    return list[index]->count();
}

QDate ActivityList::getSetDate(int index)
{
    return list[index]->getDate();
}
