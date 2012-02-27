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
    return activities.count();
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    int row = index.row();

    if(row > 0)
    {
        QDate requestedDate = currentDate.addDays(-row);

        qDebug( "row is %d", row);
        qDebug( "date is %s", requestedDate.toString().toLocal8Bit().data() );

        for(int i = 0; i < activities.size(); i++)
        {
            if(requestedDate >= activities[i]->beginDate() && requestedDate <= activities[i]->endDate())
            {
                QVariant var;
                var.setValue(activities[i]);
                return var;
            }
        }

        // if we don't have the result try blocking approach

        ActivitySet *set = source->getActivitySet(7, requestedDate, requestedDate);
        QVariant var;
        var.setValue(set);
        return var;
    }

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary
    source->startSearch(QDate::currentDate().addDays(-40), QDate::currentDate());

    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{    
    activities.append(set);
    for(int j = 0; j < set->count(); j++)
    {
        qDebug() << set->activity(j)->getUrl() << set->activity(j)->getDate();
    }
}
