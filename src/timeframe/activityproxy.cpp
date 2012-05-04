#include "activityproxy.h"
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
    connect(this, SIGNAL(newSearch(QDate, ActivitySource::Direction)), source, SLOT(startSearch(QDate, ActivitySource::Direction)));
    connect(source, SIGNAL(finishedListing()), SIGNAL(finished()));
    connect(source, SIGNAL(finishedListing()), SLOT(listingFinished()));
    connect(source, SIGNAL(newTSEntries(int, int)), SIGNAL(newMonth(int,int)));
    connect(source, SIGNAL(monthFinished(QDate)), SIGNAL(monthFinished(QDate)));

    QDate d = QDate::currentDate();

    //emit newSearch(d, ActivitySource::Left);
}

void ActivityProxy::addActivitySet(ActivitySet *set)
{
    // start generating previews for set
    QStringList files;
    for(int i = 0; i < set->count(); i++)
        files.append(set->getUrl(i));
    PreviewGenerator::instance()->start(files);

    // emit again for models
    emit newActivitySet(set);
}

void ActivityProxy::setMonth(int year, int month)
{
    QDate date;
    date.setDate(year, month + 1, 1);
    date.setDate(date.year(), date.month(), date.daysInMonth());

    if(date > QDate::currentDate())
        date = QDate::currentDate();

    emit newSearch(date, ActivitySource::Left);
}

void ActivityProxy::listingFinished()
{

}

void ActivityProxy::startNewSearch(int year, int month, bool direction)
{
    QDate date(year, month+1, 1);
    date.setDate(year, month+1, direction ? 1 : date.daysInMonth());
    qDebug() << "+++" << date;
    emit newSearch(date, direction ? ActivitySource::Right : ActivitySource::Left);
}
