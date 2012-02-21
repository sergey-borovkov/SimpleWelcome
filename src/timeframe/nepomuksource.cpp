#include "nepomuksource.h"
#include "activityset.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>

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
