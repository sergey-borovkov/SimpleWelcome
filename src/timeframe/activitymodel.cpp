#include "activitymodel.h"

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ActivityModel::~ActivityModel()
{
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
