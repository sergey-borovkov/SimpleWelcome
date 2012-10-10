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
    // start generating previews
    QStringList urls;
    foreach(Activity * item, list) {
        //item->setParent(this);
        urls.append(item->url());
        emit newMonth(item->date().year() , item->date().month(), item->type());  //fill timeScaleModel
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

