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

signals:
    void searchFinished();
    void resultCount(int);

public slots:
    virtual void startSearch(const QDate &beginDate, int direction);
    virtual void setLimit(int limit);

private:
    Direction direction;
    int m_limit;
    QList<QDate> m_searchQueue;
    QTimer* m_timer;
};

#endif // NEPOMUKSOURCE_H
