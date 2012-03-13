#include "activitylist.h"
#include "activityset.h"

#include <QtCore/QDebug>

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
    emit countChanged();
}

ActivitySet *ActivityList::at(int i) const
{
    return list.at(i);
}

QString ActivityList::getUrl(int set, int index)
{
    if(set >= list.count())
        return "";
    return list[set]->getUrl(index);
}

int ActivityList::getSetCount(int index)
{    
    if(index >= list.size())
        return 0;
    return list[index]->count();
}

QDate ActivityList::getSetDate(int index)
{
    return list[index]->getDate();
}
