#include "activityproxy.h"
#include "activitysource.h"
#include "activityset.h"
#include "activitylist.h"
#include "previewgenerator.h"

#include <QDate>
#include <QList>
#include <QDebug>
#include <QtAlgorithms>

ActivityProxy::ActivityProxy(QObject *parent) :
    QObject(parent)
{
}

ActivityProxy::~ActivityProxy()
{
}

void ActivityProxy::addSource(ActivitySource *source)
{    
    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
    connect(this, SIGNAL(newSearch(QDate)), source, SLOT(startSearch(QDate)));
    connect(source, SIGNAL(finishedListing()), SLOT(listingFinished()));

    QDate d = QDate::currentDate();

    emit newSearch(d);

}

void ActivityProxy::addActivitySet(ActivitySet *set)
{
    QDate d = set->getDate();
    d.setDate(d.year(), d.month(), 1); // set day to 1 because we only care about year and month


    // start generating previews for set
    QStringList files;
    for(int i = 0; i < set->count(); i++)
        files.append(set->getUrl(i));
    PreviewGenerator::instance()->start(files);

    // find place to insert new set
    int index = 0;
    bool insertIntoExisting = false;
    bool duplicate = false;

    // TODO use binary search
    for( ; index < activityList.size(); index++)
        if(activityList[index]->date() == d)
        {

            for(int i = 0; i < activityList[index]->count(); i++)
                if(activityList[index]->at(i)->getDate() == set->getDate())
                {
                    duplicate = true;
                    break;
                }

            if(activityList[index]->count() < 3)
                insertIntoExisting = true;

            if(duplicate || insertIntoExisting)
                break;
        }
        else if(activityList[index]->date() < d)
            continue;
        else
            break;


    if(insertIntoExisting)
    {
        activityList[index]->addSet(set);
        emit listChanged(index, activityList[index]);
    }
    else if(!duplicate) // all activitylists are full or no activity list for set's year and month
    {
        ActivityList *list = new ActivityList(set->getDate().year(), set->getDate().month(), this);
        list->addSet(set);
        activityList.insert(index, list);
        emit newList(index, list);
    }

}

void ActivityProxy::setMonth(int year, int month)
{
    QDate date;
    date.setDate(year, month + 1, 1);
    date.setDate(date.year(), date.month(), date.daysInMonth());

    if(date > QDate::currentDate())
        date = QDate::currentDate();

    emit newSearch(date);
}

void ActivityProxy::listingFinished()
{

}
