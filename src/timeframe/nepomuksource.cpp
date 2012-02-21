#include "nepomuksource.h"
#include "activityset.h"

#include <nepomuk/comparisonterm.h>
#include <nepomuk/literalterm.h>
#include <nepomuk/orterm.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/filequery.h>
#include <nepomuk/nfo.h>
#include <nepomuk/nie.h>
#include <nepomuk/resource.h>
#include <nepomuk/term.h>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>

#include <QDateTime>
#include <QFile>



NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent)
{
}

ActivitySet *NepomukSource::getActivitySet(int limit, const QDate &beginDate, const QDate &endDate)
{
    Nepomuk::Query::AndTerm term;
    Nepomuk::Query::ComparisonTerm mtime = Nepomuk::Vocabulary::NIE::lastModified() > Nepomuk::Query::LiteralTerm( QDateTime::currentDateTime().addDays(-7) );
    Nepomuk::Query::Query query( mtime );
    query.setLimit(limit);

    QList<Nepomuk::Query::Result> results = Nepomuk::Query::QueryServiceClient::syncQuery(query, NULL).count();

    ActivitySet *set = new ActivitySet;

    for(int i = 0; i < results.size(); i++)
    {

    }

    return 0;
}
