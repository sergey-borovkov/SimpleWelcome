#include "gallerylister.h"
#include "activityset.h"
#include "nepomuksource.h"

GalleryLister::GalleryLister(QObject *parent) :
    QObject(parent),
    m_source(0)
{
}

void GalleryLister::addNepomukSource(NepomukSource *source)
{
    m_source = source;
    connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SIGNAL(newActivities(QList<Activity*>)));
    //connect(this, SIGNAL(newSearch(QDate, ActivitySource::Direction)), source, SLOT(startDetailedSearch(QDate, ActivitySource::Direction)));
    //connect(source, SIGNAL(finishedListing()), SIGNAL(finished()));
    //connect(source, SIGNAL(finishedListing()), SLOT(listingFinished()));
    //connect(source, SIGNAL(newTSEntries(int, int)), SIGNAL(newMonth(int,int)));
    //connect(source, SIGNAL(monthFinished(QDate)), SIGNAL(monthFinished(QDate)));
}

void GalleryLister::startSearch(QDate date, int direction)
{
    if (!m_source)
       return;
    ActivitySource::Direction dir;
    if (direction)
    {
        dir = ActivitySource::Right;
    } else
    {
        dir = ActivitySource::Left;
    }
    m_source->startDetailedSearch(date, dir);
}
