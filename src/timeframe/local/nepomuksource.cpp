#include "nepomuksource.h"
#include "activity.h"

#include <Nepomuk/ResourceManager>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <QtGui/QApplication>
#include <QtCore/QDate>
#include <QtCore/QTimer>

NepomukSource::NepomukSource(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

void NepomukSource::startSearch()
{
    QString sparqlQuery("select ?url ?lastModified ?mimeType where { ?r nie:url ?url . ?r nie:lastModified ?lastModified . ?r nie:mimeType ?mimeType . { ?r a nfo:Document .} UNION { ?r a nfo:Image . } UNION { ?r a nfo:Video . }}");
    Soprano::Model* model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( sparqlQuery, Soprano::Query::QueryLanguageSparql );

    QList<Activity *> activities;
    int i = 0;
    while(it.next()) {
        i++;
        QString path = it["url"].uri().toLocalFile();
        QDate lastModified = it["lastModified"].literal().toDate();
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

        Activity *activity = new Activity(path, type, lastModified, mimeType);
//        activity->moveToThread(QApplication::instance()->thread());
        activities.append(activity);
    }

    if(!activities.isEmpty()) {
        emit newActivities(activities);
    }

    emit searchFinished();
}
