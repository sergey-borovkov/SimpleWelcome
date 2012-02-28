#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/FileQuery>

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
signals:
    void newEntries(const QList<Nepomuk::Query::Result>&);

public slots:
    virtual void startSearch(const QDate &beginDate, const QDate &endDate);
    void processEntry(const QList<Nepomuk::Query::Result> &list);

private:
    Nepomuk::Query::FileQuery createQuery(const QDate &beginDate, const QDate &endDate);
    ActivitySet *createActivitySet(const QList<Nepomuk::Query::Result> &result);

    Nepomuk::Query::QueryServiceClient* m_searchClient;
};

#endif // NEPOMUKSOURCE_H
