#include "activityproxy.h"
#include "localdaymodel.h"
#include "activityset.h"
#include "nepomuksource.h"
#include "previewgenerator.h"

#include <QDebug>

ActivityProxy::ActivityProxy(QObject *parent) :
    QObject(parent),
    m_source(0),
    m_model(0)
{
}

void ActivityProxy::addNepomukSource(NepomukSource *source)
{
    m_source = source;
    //connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SIGNAL(newActivities(QList<Activity*>)));
    connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newData(QList<Activity*>)));
    //connect(this, SIGNAL(newSearch(QDate, ActivitySource::Direction)), source, SLOT(startDetailedSearch(QDate, ActivitySource::Direction)));
    connect(source, SIGNAL(finishedListing()), SIGNAL(finished()));
    connect(source, SIGNAL(resultCount(int)), this, SLOT(resultCount(int)));
    //connect(source, SIGNAL(newTSEntries(int, int)), SIGNAL(newMonth(int,int)));
    //connect(source, SIGNAL(monthFinished(QDate)), SIGNAL(monthFinished(QDate)));
}

void ActivityProxy::startSearch(QDate date, int direction)
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

void ActivityProxy::resultCount(int count)
{
    //qDebug() << "Result count: " << count;
}

void ActivityProxy::newData(QList<Activity *> list)
{
    // start generating previews
    QStringList urls;
    foreach(Activity* item, list)
    {
        urls.append(item->getUrl());
        emit newMonth(item->getDate().year() , item->getDate().month(), item->getType());  //fill timeScaleModel
    }
    PreviewGenerator::instance()->start(urls);
    // send events to the model
    emit newActivities(list);
}

void ActivityProxy::setModel(LocalDayModel* model)
{
    if (m_model)
        m_model = model;
}


int ActivityProxy::getIndexByDate(int year, int month,  bool direction)
{
    if (m_model)
     return m_model->getIndexByDate(year, month, direction);
    return -1;
}

QDate ActivityProxy::getDateOfIndex(int listIndex)
{
    if (m_model)
        return m_model->getDateOfIndex(listIndex);
    return QDate();
}

