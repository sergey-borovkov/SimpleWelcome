#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/FileQuery>

#include <QDate>

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
    void newTSEntries(int year,int month);

public slots:
    virtual void startSearch(const QDate &beginDate);
    void processEntry(const QList<Nepomuk::Query::Result> &list);
    void processTSEntry(const QList<Nepomuk::Query::Result> &list);

private slots:
    void listingFinished();
    void listingTSFinished();

private:
    Nepomuk::Query::FileQuery createQuery(const QDate &beginDate);
    Nepomuk::Query::FileQuery createTimeScaleQuery(const QDate &beginDate);

    ActivitySet *createActivitySet(const QList<Nepomuk::Query::Result> &result);
    void fillTimeScaleModel(const QDate &beginDate);

    Nepomuk::Query::QueryServiceClient* m_searchClient;
    Nepomuk::Query::QueryServiceClient* m_timeScaleClient;
    QDate queryDate;
    QDate m_timeScaleDate;
    bool m_tsSearch;
};

#endif // NEPOMUKSOURCE_H
