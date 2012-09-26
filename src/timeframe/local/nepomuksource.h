#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include "activitysource.h"

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/FileQuery>

#include <QDate>
#include <QTimer>

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
    enum Mode {
        Normal = 0,
        Detailed
    };

    explicit NepomukSource(QObject *parent = 0);
    ~NepomukSource();

    // this function is thread safe    
signals:
    void newEntries(const QList<Nepomuk::Query::Result>&);
    //void newTSEntries(int year, int month);
    //void monthFinished(QDate date);
    void resultCount(int);

public slots:
    virtual void startSearch(const QDate &beginDate, ActivitySource::Direction direction);
    virtual void setLimit(int limit);
    void processEntry(const QList<Nepomuk::Query::Result> &list);
    //void processTSEntry(const QList<Nepomuk::Query::Result> &list);

private slots:
    void listingFinished();
    //void listingTSFinished();
    void error(QString str);
    void startSearchFromQueue();

private:
    Nepomuk::Query::FileQuery createQuery();
   // Nepomuk::Query::FileQuery createTimeScaleQuery(const QDate &beginDate);

    //ActivitySet *createActivitySet(const QList<Nepomuk::Query::Result> &result);
    //void fillTimeScaleModel(const QDate &beginDate);

    //QString resolveType(QString path, QList<QUrl> typesList);

    Nepomuk::Query::QueryServiceClient* m_searchClient;    
    QDate queryDate;
    //QDate m_timeScaleDate;
    //bool m_tsSearch;
    //ActivitySet *set;
    Direction direction;
    Mode m_mode;
    int m_limit;
    QList<QDate> m_searchQueue;
    QTimer* m_timer;
};

#endif // NEPOMUKSOURCE_H