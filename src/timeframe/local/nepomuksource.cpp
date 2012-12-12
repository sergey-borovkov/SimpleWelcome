/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "nepomuksource.h"
#include "activity.h"

#include <KToolInvocation>
#include <Nepomuk/ResourceManager>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>

#include <QtCore/QDate>


NepomukSource::NepomukSource(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType< QList<Activity *> >("QList<Activity*>");
}

void NepomukSource::startSearch()
{
    QString sparqlQuery("select ?url ?lastModified ?mimeType ?width ?height where "
                        "{ ?r nie:url ?url . ?r nie:lastModified ?lastModified . ?r nie:mimeType ?mimeType . "
                        "optional { ?r nfo:width ?width . ?r nfo:height ?height } . "
                        "{ ?r a nfo:Document .} UNION { ?r a nfo:Image . } UNION { ?r a nfo:Video . }}");
    Soprano::Model *model = Nepomuk::ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery(sparqlQuery, Soprano::Query::QueryLanguageSparql);

    QList<Activity *> activities;
    int i = 0;
    while (it.next()) {
        QString path = it["url"].uri().toLocalFile();
        QDate lastModified = it["lastModified"].literal().toDate();
        QString mimeType = it["mimeType"].toString();

        // skip desktop files
        if (mimeType == QLatin1String("application/x-desktop")) {
            continue;
        }

        i++;

        QString type;
        int imageWidth = 0, imageHeight = 0;
        if (path.contains(".svg") || path.contains(".mng"))
            type = "Image";
        else if (mimeType.contains("video"))
            type = "Video";
        else if (mimeType.contains("image"))
            type = "Image";
        else
            type = "Document";
        if (type == "Image") {
            imageWidth = it["width"].toString().toInt();
            imageHeight = it["height"].toString().toInt();
        }

        if (i %= 100) {
            emit newActivities(activities);
            activities.clear();
        }

        Activity *activity = new Activity(path, type, lastModified, mimeType, QSize(imageWidth, imageHeight));
        activities.append(activity);
    }

    if (!activities.isEmpty()) {
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
