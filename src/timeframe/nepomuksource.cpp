#include "nepomuksource.h"
#include "activityset.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/File>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Vocabulary/NIE>

#include <QFile>
#include <QFileInfo>
#include <QDateTime>

NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent), m_searchClient(0) , m_timeScaleClient(0), m_tsSearch(false), set(0), m_monthChanged(false)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

NepomukSource::~NepomukSource()
{
    if(m_searchClient)
        m_searchClient->close();
    if(m_timeScaleClient)
        m_timeScaleClient->close();
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
    Nepomuk::Query::ComparisonTerm endDateTerm = Nepomuk::Vocabulary::NIE::lastModified() < Nepomuk::Query::LiteralTerm( date.addDays(1) );
    Nepomuk::Query::ComparisonTerm image(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("image/"));
    Nepomuk::Query::AndTerm term(beginDateTerm,endDateTerm, image);

    Nepomuk::Query::FileQuery query(term);

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
    /*
    if (beginDate.isNull())
        qDebug("----------Null Date");
        */
    qDebug() <<"start new search at " << beginDate << direction;
    this->direction = direction;

    if(!m_searchClient)
    {
        m_searchClient = new Nepomuk::Query::QueryServiceClient( this );

        connect(m_searchClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)), SLOT(processEntry(const QList<Nepomuk::Query::Result> &)));

        connect(m_searchClient, SIGNAL(finishedListing()), SLOT(listingFinished()));

    }

    if(!m_tsSearch)
    {
        m_tsSearch = true;
        fillTimeScaleModel( QDate::currentDate());
    }

    queryDate = beginDate;

    set = new ActivitySet;

    Nepomuk::Query::Query query = createQuery(beginDate);

    query.setLimit(7);

    m_searchClient->query(query);
}


void NepomukSource::processEntry(const QList<Nepomuk::Query::Result> &list)
{
    QList<Activity *> activities;
    for(int i = 0; i < list.size(); i++)
    {
        QString uri = list.at(i).resource().toFile().url().path();
        QString type = list.at(i).resource().type();
        QFileInfo fi(uri);
        activities.append(new Activity(uri, type, fi.lastModified().date()));
        set->addActivity(new Activity(uri, type, fi.lastModified().date()));
   }

   emit newActivities(activities);
}

void NepomukSource::listingFinished()
{
    //if we got any data in last search - emit it
    if(set->count())
    {
        set->setDate(queryDate);
        emit newActivitySet(set);
    }

    int delta = ( direction == Right ) ? 1 : -1;

    // month changed on this step so we emit previous month
    if(m_monthChanged)
    {
        QDate d = queryDate.addDays(-delta);
        d.setDate(d.year(), d.month(), 1);

        emit monthFinished(d);
        m_monthChanged = false;
    }

    // continue search if any days in month left
    if( queryDate.month() == queryDate.addDays(delta).month() )
    {
        //m_searchClient->close();
        //m_searchClient = 0;

        return startSearch(queryDate.addDays( delta ), direction);
    }

    // next time this function is entered month's gonna change
    m_monthChanged = true;

    emit finishedListing();
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
    Nepomuk::Query::AndTerm term(beginDateTerm,endDateTerm, image);
    Nepomuk::Query::FileQuery query(term);
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
    if (list.count()> 0)
        emit newTSEntries(m_timeScaleDate.year(),m_timeScaleDate.month());
}
