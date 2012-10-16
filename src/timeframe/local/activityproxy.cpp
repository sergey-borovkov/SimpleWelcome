#include "activityproxy.h"
#include "activity.h"
#include "localcontentmodel.h"
#include "nepomuksource.h"
#include "previewgenerator.h"

ActivityProxy::ActivityProxy(NepomukSource *source, QObject *parent) :
    QObject(parent),
    m_model(0)
{
    connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newData(QList<Activity*>)));
    connect(source, SIGNAL(searchFinished()), SIGNAL(finished()));
    connect(this, SIGNAL(search()), source, SLOT(startSearch()));
}

void ActivityProxy::startSearch()
{
    emit search();
}

void ActivityProxy::newData(QList<Activity *> list)
{
    foreach(Activity * item, list) {
        emit newMonth(item->date().year() , item->date().month(), item->type());  //fill timeScaleModel
    }
    emit newActivities(list);
}

void ActivityProxy::setModel(LocalContentModel* model)
{
    if (m_model)
        m_model = model;
}

