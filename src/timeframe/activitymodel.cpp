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

    QHashIterator<int, QByteArray> i(roles);
    while (i.hasNext()) {
        i.next();
        qDebug("role %d: %s ", i.key(), i.value().data() );
    }
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
    source->startSearch(QDate::currentDate().addDays(-30), QDate::currentDate());
    connect(source, SIGNAL(newActivities(QList<Activity*>)), SLOT(addActivities(QList<Activity*>)));
}

void ActivityModel::addActivities(QList<Activity *> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        if(!map.contains(list[i]->getDate()))
        {
            int first = list[i]->getDate().daysTo(currentDate);
//            days++;
            if(first > days)
                days = first;

            if(first > rowCount(QModelIndex()))
                first = rowCount(QModelIndex()) - 1;

            beginInsertRows(QModelIndex(), first, first);
            map.insert(list[i]->getDate(), list[i]);
            endInsertRows();            
        }

        map.insert(list[i]->getDate(), list[i]);
    }
}
