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
    ActivitySource(parent)
{
}

ActivitySet *NepomukSource::getActivitySet(int limit, const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::Query::Query query = createQuery(beginDate, endDate);
    query.setLimit(limit);

    QList<Nepomuk::Query::Result> results = Nepomuk::Query::QueryServiceClient::syncQuery(query, NULL);
    return createActivitySet(results);
}


Nepomuk::Query::FileQuery NepomukSource::createQuery(const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::Query::ComparisonTerm beginDateTerm = Nepomuk::Vocabulary::NIE::lastModified() >= Nepomuk::Query::LiteralTerm( beginDate );
    Nepomuk::Query::ComparisonTerm endDateTerm = Nepomuk::Vocabulary::NIE::lastModified() <= Nepomuk::Query::LiteralTerm( endDate );
    Nepomuk::Query::ComparisonTerm image(Nepomuk::Vocabulary::NIE::mimeType(), Nepomuk::Query::LiteralTerm("image"));

    Nepomuk::Query::AndTerm term(beginDateTerm, endDateTerm, image);
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
            QString uri = result.at(i).resource().toFile().url().path();  //result.at(i).resource().uri();
            QString type = result.at(i).resource().type();
            QFileInfo fi(uri);
            set->addActivity(uri, type, fi.lastModified().date());
        }
    }

    return set;
}

void NepomukSource::startSearch(const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::Query::Query query = createQuery(beginDate, endDate);

    m_searchClient = new Nepomuk::Query::QueryServiceClient( this );
    connect(m_searchClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)), SLOT(processEntry(const QList<Nepomuk::Query::Result> &)));
    m_searchClient->query(query);
}

void NepomukSource::processEntry(const QList<Nepomuk::Query::Result> &list)
{
    //ActivitySet *set = createActivitySet(list);
    //emit newActivitySet(set);
    QList<Activity *> activities;
    for(int i = 0; i < list.size(); i++)
    {
        QString uri = list.at(i).resource().toFile().url().path();  //result.at(i).resource().uri();
        QString type = list.at(i).resource().type();
        QFileInfo fi(uri);
        activities.append(new Activity(uri, type, fi.lastModified().date()));
   }

    emit newActivities(activities);
}
