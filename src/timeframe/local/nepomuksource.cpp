#include "nepomuksource.h"
#include "activity.h"

#include <KToolInvocation>
#include <Nepomuk/ResourceManager>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <QtCore/QDate>
#include <QtCore/QDebug>


NepomukSource::NepomukSource(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType< QList<Activity*> >("QList<Activity*>");
}

void NepomukSource::startSearch()
{
    QString sparqlQuery("select ?url ?lastModified ?mimeType ?width ?height where "
                        "{ ?r nie:url ?url . ?r nie:lastModified ?lastModified . ?r nie:mimeType ?mimeType . "
                        "optional { ?r nfo:width ?width . ?r nfo:height ?height } . "
                        "{ ?r a nfo:Document .} UNION { ?r a nfo:Image . } UNION { ?r a nfo:Video . }}");
    Soprano::Model* model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery( sparqlQuery, Soprano::Query::QueryLanguageSparql );

    QList<Activity *> activities;
    int i = 0;
    while(it.next()) {
        i++;
        QString path = it["url"].uri().toLocalFile();
        QDate lastModified = it["lastModified"].literal().toDate();
        QString mimeType = it["mimeType"].toString();
        int imageWidth = it["width"].toString().toInt();
        int imageHeight = it["height"].toString().toInt();

        QString type;
        if (path.contains(".svg"))
            type = "Image";
        else if (mimeType.contains("video"))
            type = "Video";
        else if (mimeType.contains("image"))
            type = "Image";
        else
            type = "Document";

//        qDebug() << "     " << type << ":   " << imageWidth << "x" << imageHeight;

        if(i %= 100) {
            emit newActivities(activities);
            activities.clear();
        }

        Activity *activity = new Activity(path, type, lastModified, mimeType, QSize(imageWidth, imageHeight));
        activities.append(activity);
    }

    if(!activities.isEmpty()) {
        emit newActivities(activities);
    }

    emit searchFinished();
}

bool NepomukSource::isNepomukInitialized()
{
    return Nepomuk::ResourceManager::instance()->initialized();
}

void NepomukSource::nepomukConfigure()
{
    QStringList args;
    args << "kcm_nepomuk";
    KToolInvocation::kdeinitExec("kcmshell4", args);
}
