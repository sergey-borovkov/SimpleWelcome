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

signals:
    void newEntries(const QList<Nepomuk::Query::Result>&);
    void resultCount(int);

public slots:
    virtual void startSearch(const QDate &beginDate, ActivitySource::Direction direction);
    virtual void setLimit(int limit);
    void processEntry(const QList<Nepomuk::Query::Result> &list);

private slots:
    void listingFinished();
    void error(QString str);
    void startSearchFromQueue();

private:
    Nepomuk::Query::FileQuery createQuery();
    Nepomuk::Query::QueryServiceClient* m_searchClient;
    QDate queryDate;
    Direction direction;
    Mode m_mode;
    int m_limit;
    QList<QDate> m_searchQueue;
    QTimer* m_timer;
};

#endif // NEPOMUKSOURCE_H
