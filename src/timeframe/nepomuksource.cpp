#include "nepomuksource.h"
#include "activityset.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Vocabulary/NIE>

#include <QFile>
#include <QDateTime>

NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent)
{
}

ActivitySet *NepomukSource::getActivitySet(int limit, const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::Query::ComparisonTerm beginDateTerm = Nepomuk::Vocabulary::NIE::lastModified() >= Nepomuk::Query::LiteralTerm( beginDate );
    Nepomuk::Query::ComparisonTerm endDateTerm = Nepomuk::Vocabulary::NIE::lastModified() <= Nepomuk::Query::LiteralTerm( endDate );

    Nepomuk::Query::AndTerm term(beginDateTerm, endDateTerm);
    Nepomuk::Query::Query query( term );
    query.setLimit(limit);

    QList<Nepomuk::Query::Result> results = Nepomuk::Query::QueryServiceClient::syncQuery(query, NULL);

    ActivitySet *set = new ActivitySet;

    for(int i = 0; i < results.size(); i++)
    {
        QString uri = results.at(i).resource().uri();
        QString type = results.at(i).resource().type();
        set->addActivity(uri, type);
    }

    return set;
}

void NepomukSource::startSearch(const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::ResourceManager::instance()->init();
    Nepomuk::Query::ComparisonTerm mtime
            = Nepomuk::Vocabulary::NIE::lastModified() > Nepomuk::Query::LiteralTerm( QDateTime::currentDateTime().addDays(-365) );

    Nepomuk::Query::Query query( mtime );

    m_searchClient = new Nepomuk::Query::QueryServiceClient( this );

    connect(m_searchClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)),this, SLOT(slotNewEntries(const QList<Nepomuk::Query::Result>&)));

    connect( m_searchClient, SIGNAL( resultCount(int) ), this, SLOT( slotTotalCount(int) ) );

    m_searchClient->query(query);

}
