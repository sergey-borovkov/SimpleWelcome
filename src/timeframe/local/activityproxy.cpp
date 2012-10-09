#include "activityproxy.h"
#include "activityset.h"
#include "localcontentmodel.h"
#include "nepomuksource.h"
#include "previewgenerator.h"

ActivityProxy::ActivityProxy(QObject *parent) :
    QObject(parent),
    m_model(0),
    m_source(0)
{
}

void ActivityProxy::addNepomukSource(NepomukSource *source)
{
    m_source = source;
    connect(source, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newData(QList<Activity*>)));
    connect(source, SIGNAL(searchFinished()), SIGNAL(finished()));
    connect(this, SIGNAL(search()), source, SLOT(startSearch()));
}

void ActivityProxy::startSearch()
{
    if (!m_source)
        return;
    emit search();
}

void ActivityProxy::newData(QList<Activity *> list)
{
    // start generating previews
    QStringList urls;
    foreach(Activity * item, list) {
        item->setParent(this);
        urls.append(item->getUrl());
        emit newMonth(item->getDate().year() , item->getDate().month(), item->getType());  //fill timeScaleModel
    }
    PreviewGenerator::instance()->start(urls);
    // send events to the model
    emit newActivities(list);
}

void ActivityProxy::setModel(LocalContentModel* model)
{
    if (m_model)
        m_model = model;
}


int ActivityProxy::getIndexByDate(int year, int month,  bool direction)
{
    if (m_model)
        return m_model->getIndexByDate(year, month, direction);
    return -1;
}

QDate ActivityProxy::getDateOfIndex(int listIndex)
{
    if (m_model)
        return m_model->getDateOfIndex(listIndex);
    return QDate();
}

