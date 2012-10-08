#include "activityset.h"
#include "nepomuksource.h"

#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Vocabulary/NFO>
#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/ResourceManager>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <QtCore/QDate>
#include <QtCore/QTimer>

using namespace Nepomuk::Vocabulary;
using namespace Nepomuk::Query;

NepomukSource::NepomukSource(QObject *parent) :
    ActivitySource(parent), m_limit(0), m_timer(0)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

void NepomukSource::startSearch(const QDate &beginDate, int direction)
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

    this->direction = NepomukSource::Right;

    QString sparqlQuery("select ?url ?lastModified ?mimeType where { ?r nie:url ?url . ?r nie:lastModified ?lastModified . ?r nie:mimeType ?mimeType . { ?r a nfo:Document .} UNION { ?r a nfo:Image . } UNION { ?r a nfo:Video . }}");
    Soprano::Model* model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( sparqlQuery, Soprano::Query::QueryLanguageSparql );

    QList<Activity *> activities;
    int i = 0;
    while(it.next()) {
        i++;
        QString path = it["url"].uri().toLocalFile();
        QString mimeType = it["mimeType"].toString();
        QString type;
        if (path.contains(".svg"))
            type = "Image";
        else if (mimeType.contains("video"))
            type = "Video";
        else if (mimeType.contains("image"))
            type = "Image";
        else
            type = "Document";

        if(i %= 100) {
            emit newActivities(activities);
            activities.clear();
        }

        activities.append(new Activity(path, type ,it["lastModified"].literal().toDate()));
    }

    if(!activities.isEmpty()) {
        emit newActivities(activities);
    }


    if (!m_timer) {
        m_timer = new QTimer(this);
    }
    m_timer->start(1000 * 60 * 10); //One query in ten minutes
    emit searchFinished();
}


void NepomukSource::setLimit(int limit)
{
    m_limit = limit;
}
