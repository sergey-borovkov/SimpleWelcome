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

    Nepomuk::Query::AndTerm term(beginDateTerm, endDateTerm);
    Nepomuk::Query::FileQuery query(term);

    return query;
}

ActivitySet *NepomukSource::createActivitySet(const QList<Nepomuk::Query::Result> &result)
{
    ActivitySet *set = new ActivitySet;
    qDebug() << "RESULT SIZE" << result.size();
    for(int i = 0; i < result.size(); i++)
    {
        if(result.at(i).resource().isFile())
        {
            Nepomuk::File file = result.at(i).resource().toFile();

            QString uri = result.at(i).resource().toFile().url().path();  //result.at(i).resource().uri();
            QString type = result.at(i).resource().type();

            set->addActivity(uri, type);
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
//    qDebug() << "Processing entry";
    ActivitySet *set = createActivitySet(list);
    emit newActivitySet(set);
}
