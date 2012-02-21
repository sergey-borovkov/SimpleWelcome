#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

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
    void startSearch();
signals:
    
public slots:

private:
    Nepomuk::Query::QueryServiceClient* m_searchClient;
};

#endif // NEPOMUKSOURCE_H
