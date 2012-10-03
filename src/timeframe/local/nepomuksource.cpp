#include "activityset.h"
#include "nepomuksource.h"

#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Vocabulary/NFO>
#include <Nepomuk/Vocabulary/NIE>

#include <QtCore/QDate>
#include <QtCore/QTimer>

using namespace Nepomuk::Vocabulary;
using namespace Nepomuk::Query;

NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent), m_searchClient(0), m_limit(0), m_timer(0)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

NepomukSource::~NepomukSource()
{
    if (m_searchClient)
        m_searchClient->close();
    delete m_searchClient;
    qDeleteAll(m_activities);
    m_activities.clear();
}

Nepomuk::Query::FileQuery NepomukSource::createQuery()
{
    ResourceTypeTerm image = ResourceTypeTerm(NFO::Image());
    ResourceTypeTerm video = ResourceTypeTerm(NFO::Video());
    ResourceTypeTerm document = ResourceTypeTerm(NFO::Document());

    Nepomuk::Query::OrTerm orTerm(video, image, document);

    Nepomuk::Query::FileQuery query(orTerm);

    query.addRequestProperty(Query::RequestProperty(NIE::lastModified()));
    query.addRequestProperty(Query::RequestProperty(NIE::url()));
    query.addRequestProperty(Query::RequestProperty(NIE::mimeType()));

    return query;
}

void NepomukSource::startSearch(const QDate &beginDate, Direction direction)
{
    Q_UNUSED(direction)
    if (m_searchQueue.size() == 0) {
        m_searchQueue.append(beginDate);
        startSearchFromQueue();
    } else
        m_searchQueue.append(beginDate);
}

void NepomukSource::startSearchFromQueue()
{
    if (m_timer) {
        if (m_timer->isActive()) {
            emit finishedListing();
            return;
        }
    }

    m_mode = Normal;
    this->direction = NepomukSource::Right;

    if (m_searchClient) {
        m_searchClient->close();
        m_searchClient->deleteLater();
    }

    m_searchClient = new QueryServiceClient();

    connect(m_searchClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result>&)), SLOT(processEntry(const QList<Nepomuk::Query::Result> &)));
    connect(m_searchClient, SIGNAL(finishedListing()), SLOT(listingFinished()));
    connect(m_searchClient, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(m_searchClient, SIGNAL(resultCount(int)), this, SIGNAL(resultCount(int)));


    Nepomuk::Query::Query query = createQuery();

    query.setLimit(m_limit);

    m_searchClient->query(query);
    if (!m_timer) {
        m_timer = new QTimer(this);
    }
    m_timer->start(1000 * 60 * 10); //One query in ten minutes
}


void NepomukSource::setLimit(int limit)
{
    m_limit = limit;
}


void NepomukSource::processEntry(const QList<Nepomuk::Query::Result> &list)
{
    for (int i = 0; i < list.size(); i++) {
        Result result = list.at(i);
        QDate creationDate;
        QUrl fileurl;
        QString mimeType;

        if (result[NIE::lastModified()].isLiteral()) {
            creationDate = result[NIE::lastModified()].literal().toDate();
        } else
            continue;
        if (result[NIE::url()].isValid()) {
            fileurl = result[NIE::url()].toString();
        } else
            continue;
        if (result[NIE::mimeType()].isLiteral()) {
            mimeType = result[NIE::mimeType()].literal().toString();
        } else
            continue;

        QString type;
        QString path = fileurl.path();

        if (path.contains(".svg"))
            type = "Image";
        else if (mimeType.contains("video"))
            type = "Video";
        else if (mimeType.contains("image"))
            type = "Image";
        else
            type = "Document";

       m_activities.append(new Activity(path, type, creationDate));
    }
}

void NepomukSource::error(QString str)
{
    qDebug() << "lister error:" << str;
}

void NepomukSource::listingFinished()
{
    emit newActivities(m_activities);
    m_activities.clear();
    emit searchFinished();
}

