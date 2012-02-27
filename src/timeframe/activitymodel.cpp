#include "activitymodel.h"
#include "activitysource.h"

#include <QDate>
#include <QDebug>
#include <QHash>

#include "activityset.h"

const int ActivityModel::CurrentDateRole = Qt::UserRole + 1;
const int ActivityModel::ActivitiesRole = Qt::UserRole + 2;


ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent)
{
    currentDate = QDate::currentDate();

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
    return map.count();
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

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
             qDebug() << "values" << values.size();

             QVariant var;
             var.setValue(values);
             return var;
        }
    }

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary
    source->startSearch(QDate::currentDate().addDays(-10), QDate::currentDate());
    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
    connect(source, SIGNAL(newActivities(QList<Activity*>)), SLOT(addActivities(QList<Activity*>)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{

    // just to get it working

}

void ActivityModel::addActivities(QList<Activity *> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        if(!map.contains(list[i]->getDate()))
        {
            int first = list[i]->getDate().daysTo(currentDate);
            if(first > rowCount(QModelIndex()))
                first = rowCount(QModelIndex()) - 1;

            beginInsertRows(QModelIndex(), first, first);
            map.insert(list[i]->getDate(), list[i]);
            endInsertRows();
        }

        map.insert(list[i]->getDate(), list[i]);
    }
}
