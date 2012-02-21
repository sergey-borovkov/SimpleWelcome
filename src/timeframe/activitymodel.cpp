#include "activitymodel.h"
#include "activitysource.h"
ActivityModel::ActivityModel(QObject *parent) :
    QAbstractItemModel(parent)
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
    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{
    activities.append(set);
}
