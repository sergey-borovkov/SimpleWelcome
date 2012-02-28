#include "activitymodel.h"
#include "activitysource.h"
#include "activityset.h"

#include <QDate>
#include <QDebug>

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent), currentDate(QDate::currentDate()), days(0)
{

    QHash<int, QByteArray> roles = roleNames();
    roles.insert(CurrentDateRole, QByteArray("currentDate"));
    roles.insert(ActivitiesRole, QByteArray("activities"));
    setRoleNames(roles);
    qDebug() << roles;

}

ActivityModel::~ActivityModel()
{
}

int ActivityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return days;
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row >= 0)
    {
        if ( role == CurrentDateRole )
        {
            return QDate::currentDate().addDays(-row);
        }
        else if ( role == ActivitiesRole )
        {
            QDate requestedDate = QDate::currentDate().addDays(-row);

            qDebug( "row is %d", row);
            qDebug( "date is %s", requestedDate.toString().toLocal8Bit().data() );

             QList<Activity *> values = map.values(requestedDate);
             ActivitySet *set = new ActivitySet(values, 0);
             QVariant var;
             var.setValue(set);
             return var;
        }
    }

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary

    connect(source, SIGNAL(newActivities(QList<Activity*>)), SLOT(addActivities(QList<Activity*>)));
    connect(this, SIGNAL(newSearch(QDate, QDate)), source, SLOT(startSearch(QDate,QDate)));
    QDate d1 = QDate::currentDate().addDays(-30);
    emit newSearch(d1, currentDate);
}

void ActivityModel::addActivities(QList<Activity *> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        int row = list[i]->getDate().daysTo(currentDate);
        if(!map.contains(list[i]->getDate()) && row > days)
        {
                days = row--;
                beginInsertRows(QModelIndex(), row, row + 1);
                map.insert(list[i]->getDate(), list[i]);
                endInsertRows();
        }
        else
        {
            map.insert(list[i]->getDate(), list[i]);
            emit dataChanged(index(row), index(row));
        }
    }
}
