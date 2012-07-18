#include "gallerylister.h"
#include "activityset.h"
#include "nepomuksource.h"
#include "previewgenerator.h"

#include <QDebug>

GalleryLister::GalleryLister(QObject *parent) :
    QObject(parent),
    m_source(0)
{
}

void GalleryLister::addNepomukSource(NepomukSource *source)
{
    m_source = source;
    //connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SIGNAL(newActivities(QList<Activity*>)));
    connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newData(QList<Activity*>)));
    //connect(this, SIGNAL(newSearch(QDate, ActivitySource::Direction)), source, SLOT(startDetailedSearch(QDate, ActivitySource::Direction)));
    //connect(source, SIGNAL(finishedListing()), SIGNAL(finished()));
    connect(source, SIGNAL(resultCount(int)), this, SLOT(resultCount(int)));
    connect(source, SIGNAL(newTSEntries(int, int)), SIGNAL(newMonth(int,int)));
    //connect(source, SIGNAL(monthFinished(QDate)), SIGNAL(monthFinished(QDate)));
}

void GalleryLister::startSearch(QDate date, int direction)
{
    QDate d = date;
    if (!m_source)
       return;
    ActivitySource::Direction dir;
    if (direction)
    {
        dir = ActivitySource::Right;
        d.setDate(d.year(),d.month(),1);

    } else
    {
        dir = ActivitySource::Left;
        d.setDate(d.year(),d.month(),d.daysInMonth());
    }    
    //qDebug() << d;
    m_source->setLimit(0);
    m_source->startSearch(d, dir);
}

void GalleryLister::resultCount(int count)
{
//    qDebug() << "Result count: " << count;
}

void GalleryLister::newData(QList<Activity *> list)
{
    // start generating previews
    QStringList urls;
    foreach(Activity* item, list)
    {
        urls.append(item->getUrl());
    }
    PreviewGenerator::instance()->start(urls);
    // send events to the model
    emit newActivities(list);
}
