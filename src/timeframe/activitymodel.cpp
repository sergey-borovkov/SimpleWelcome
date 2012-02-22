#include "activitymodel.h"
#include "activitysource.h"

#include <QDate>
#include <QDebug>

#include "activityset.h"

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ActivityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return activities.count();
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary
    source->startSearch(QDate::currentDate().addDays(-3), QDate::currentDate());


    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{    
    qDebug() << "COUNT" << set->count();

    for(int i = 0; i < set->count(); i++)
    {
        qDebug() << set->activity(i)->getUrl() << set->activity(i)->getType();
    }
    qDebug() << "END";

    activities.append(set);
}
