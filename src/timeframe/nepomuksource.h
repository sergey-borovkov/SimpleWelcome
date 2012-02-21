#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Result>

namespace Nepomuk
{
    namespace Query
    {
        class QueryServiceClient;
    }
}

class NepomukSource : public ActivitySource
{
    Q_OBJECT
public:
    explicit NepomukSource(QObject *parent = 0);
    // this function is thread safe
    ActivitySet *getActivitySet(int limit, const QDate &beginDate, const QDate &endDate);
    void startSearch(const QDate &beginDate, const QDate &endDate);
signals:
    void newEntries(const QList<Nepomuk::Query::Result>&);

public slots:

private:
    Nepomuk::Query::Query createQuery(const QDate &beginDate, const QDate &endDate);

    Nepomuk::Query::QueryServiceClient* m_searchClient;
};

#endif // NEPOMUKSOURCE_H
