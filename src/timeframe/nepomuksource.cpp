#include "nepomuksource.h"
#include "activityset.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/File>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Vocabulary/NIE>

#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <unistd.h>

NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent), m_searchClient(0) , m_timeScaleClient(0), m_tsSearch(false), set(0), m_limit(0)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

NepomukSource::~NepomukSource()
{
    if(m_searchClient)
        m_searchClient->close();
    if(m_timeScaleClient)
        m_timeScaleClient->close();
    m_searchClient = 0;
}

ActivitySet *NepomukSource::getActivitySet(int limit, const QDate &beginDate, const QDate &endDate)
{
    Q_UNUSED(endDate)
    Nepomuk::Query::Query query = createQuery(beginDate);
    query.setLimit(limit);

    QList<Nepomuk::Query::Result> results = Nepomuk::Query::QueryServiceClient::syncQuery(query, NULL);
    return createActivitySet(results);
}


Nepomuk::Query::FileQuery NepomukSource::createQuery(const QDate &date)
{
    Nepomuk::Query::ComparisonTerm beginDateTerm = Nepomuk::Vocabulary::NIE::lastModified() >= Nepomuk::Query::LiteralTerm( date );
    Nepomuk::Query::ComparisonTerm endDateTerm = Nepomuk::Vocabulary::NIE::lastModified() < Nepomuk::Query::LiteralTerm( date.addDays(date.daysInMonth()) );
    Nepomuk::Query::ComparisonTerm image(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("image/"));
    Nepomuk::Query::ComparisonTerm video(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("video/"));

    Nepomuk::Query::AndTerm term1(beginDateTerm,endDateTerm, image);
    Nepomuk::Query::AndTerm term2(beginDateTerm,endDateTerm, video);

    Nepomuk::Query::OrTerm orTerm(term1,term2);

    Nepomuk::Query::FileQuery query(orTerm);
    return query;
}

ActivitySet *NepomukSource::createActivitySet(const QList<Nepomuk::Query::Result> &result)
{
    ActivitySet *set = new ActivitySet;

    for(int i = 0; i < result.size(); i++)
    {
        if(result.at(i).resource().isFile())
        {
            QString uri = result.at(i).resource().toFile().url().path();
            QString type = result.at(i).resource().type();
            QFileInfo fi(uri);
            set->addActivity(uri, type, fi.lastModified().date());
        }
    }

    return set;
}

void NepomukSource::startSearch(const QDate &beginDate, Direction direction)
{
    if (m_searchQueue.size()==0)
    {
        m_searchQueue.append(beginDate);
        startSearchFromQueue();
    }else
        m_searchQueue.append(beginDate);
}

void NepomukSource::setLimit(int limit)
{
    m_limit = limit;
}

void NepomukSource::processEntry(const QList<Nepomuk::Query::Result> &list)
{
    QList<Activity *> activities;
    for(int i = 0; i < list.size(); i++)
    {        
        //qDebug() << "------before resource" << i;
        Nepomuk::Query::Result result = list.at(i);
        //qDebug() << "after result" << i;
        Nepomuk::Resource res = result.resource();
        if (!res.isFile())
            continue;
        //qDebug() << "after result" << i;
        QString uri = res.toFile().url().path();

        //qDebug() << "after path";
        QString type = list.at(i).resource().type();
        //qDebug() << "after type";
        QFileInfo fi(uri);
        activities.append(new Activity(uri, type, fi.lastModified().date()));
        //qDebug() << "-----after resorce";
        set->addActivity(new Activity(uri, type, fi.lastModified().date()));
   }

   emit newActivities(activities);
}

void NepomukSource::error(QString str)
{
    qDebug() << "lister error:" << str;
}

void NepomukSource::listingFinished()
{    
    //if we got any data in last search - emit it
    if(set->count())
    {
        set->setDate(queryDate);
        emit newActivitySet(set);
    }

    //int delta = ( direction == Right ) ? 1 : -1;

    // continue search if any days in month left
    /*
    if( queryDate.month() == queryDate.addDays(delta).month() )
    {        
        return startSearch(queryDate.addDays( delta ), direction);
    }
    */



    // month changed on this step so we emit previous month
    QDate d = queryDate;
    d.setDate(d.year(), d.month(), 1);
    emit monthFinished(d);
    emit finishedListing();     

    m_searchQueue.removeFirst();
    if( m_searchQueue.size()>0 )
    {
        QTimer::singleShot(1, this, SLOT(startSearchFromQueue()));
        return;
    }
}


void NepomukSource::fillTimeScaleModel(const QDate &date)
{

    if(m_timeScaleClient)
    {
        m_timeScaleClient->close();
        m_timeScaleClient = 0;
    }

    m_timeScaleDate = date;
    //Nepomuk::Query::Query query = createQuery(beginDate);

    Nepomuk::Query::Query query = createTimeScaleQuery(date);
    query.setLimit(1);
    m_timeScaleClient = new Nepomuk::Query::QueryServiceClient( this );

    connect(m_timeScaleClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)), SLOT(processTSEntry(const QList<Nepomuk::Query::Result> &)));

    connect(m_timeScaleClient, SIGNAL(finishedListing()), SLOT(listingTSFinished()));

    m_timeScaleClient->query(query);

}

Nepomuk::Query::FileQuery NepomukSource::createTimeScaleQuery(const QDate &date)
{
    Q_UNUSED(date)

    QDate beginDate(m_timeScaleDate.year(),m_timeScaleDate.month(),1);
    QDate endDate = beginDate.addMonths(1);
    Nepomuk::Query::ComparisonTerm beginDateTerm = Nepomuk::Vocabulary::NIE::lastModified() >= Nepomuk::Query::LiteralTerm( beginDate );
    Nepomuk::Query::ComparisonTerm endDateTerm = Nepomuk::Vocabulary::NIE::lastModified() < Nepomuk::Query::LiteralTerm( endDate );
    Nepomuk::Query::ComparisonTerm image(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("image/"));    
    Nepomuk::Query::ComparisonTerm video(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("video/"));

    Nepomuk::Query::AndTerm term1(beginDateTerm,endDateTerm, image);
    Nepomuk::Query::AndTerm term2(beginDateTerm,endDateTerm, video);

    Nepomuk::Query::OrTerm orTerm(term1,term2);

    Nepomuk::Query::FileQuery query(orTerm);
    return query;
}

void NepomukSource::listingTSFinished()
{

    QDate date = m_timeScaleDate.addMonths(-1);
    if (date.year() > 1970)
    {
        fillTimeScaleModel(date);
    }else
    {
        m_timeScaleClient->close();
        m_timeScaleClient = 0;
    }
}

void NepomukSource::processTSEntry(const QList<Nepomuk::Query::Result> &list)
{
    if (list.count() > 0)
        emit newTSEntries(m_timeScaleDate.year(),m_timeScaleDate.month());
/*
    Nepomuk::Query::Result result = list.at(0);
    QFileInfo fi(result.resource().toFile().url().path());
    QDate date = fi.lastModified().date();
*/
}

void NepomukSource::startSearchFromQueue()
{
    m_mode = Normal;
    this->direction = NepomukSource::Right;

    if(m_searchClient)
    {
       m_searchClient->close();
       m_searchClient->deleteLater();
    }

    m_searchClient = new Nepomuk::Query::QueryServiceClient();

    connect(m_searchClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)), SLOT(processEntry(const QList<Nepomuk::Query::Result> &)));

    connect(m_searchClient, SIGNAL(finishedListing()), SLOT(listingFinished()));

    connect(m_searchClient, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(m_searchClient, SIGNAL(resultCount(int)), this, SIGNAL(resultCount(int)));

    if(!m_tsSearch)
    {
        m_tsSearch = true;
        fillTimeScaleModel( QDate::currentDate());
    }

    //queryDate = beginDate;

    set = new ActivitySet;

    //Nepomuk::Query::Query query = createQuery(beginDate);
    Nepomuk::Query::Query query = createQuery(m_searchQueue.first());

    query.setLimit(m_limit);

    m_searchClient->query(query);
}


