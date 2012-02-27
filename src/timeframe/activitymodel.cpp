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

    if(row >= 0)
    {
        if ( role == CurrentDateRole )
        {
            return QDate::currentDate().addDays(-5);
        }
        else if ( role == ActivitiesRole )
        {
            QDate requestedDate = QDate::currentDate().addDays(-5);//.addDays(-row);

            qDebug( "row is %d", row);
            qDebug( "date is %s", requestedDate.toString().toLocal8Bit().data() );

            for(int i = 0; i < activities.size(); i++)
            {
                if(requestedDate != activities[i]->getDate())
                {
                    QVariant var;
                    var.setValue(activities[i]);
                    return var;
                }
            }

            // if we don't have the result try blocking approach

       /*    ActivitySet *set = source->getActivitySet(7, requestedDate, requestedDate);
            QVariant var;
            var.setValue(set);
            return var;
            */
        }
    }

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary
    source->startSearch(QDate::currentDate().addDays(-6), QDate::currentDate());
    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{
    // just to get it working
    if(!activities.size())
    {
        beginInsertRows(QModelIndex(), 0, 0);
        activities.append(set);

        endInsertRows();
    }    
    else
    {
        for(int i = 0; i < set->count(); i++)
        {

            for(int j = 0; j < activities.size(); j++)
            {
                bool isFound = false;
                for(int k = 0; k < activities[j]->count() && !isFound; k++)
                {
                    if(activities[j]->activity(k)->getDate() == set->activity(i)->getDate())
                    {                        
                        activities[j]->addActivity(set->activity(i));
                        emit dataChanged(index(j, 0), index(j, 0));
                        isFound = true;
                        break;
                    }

                    if(!isFound)
                    {
                        ActivitySet *newSet = new ActivitySet;
                        newSet->addActivity(set->activity(i));
                        int rows = rowCount(QModelIndex());
                        beginInsertRows(QModelIndex(), rows - 1, rows - 1);
                        activities.append(newSet);
                        endInsertRows();
                    }
                }
            }
        }
    }
    /*
    for(int i = 0 ; i < activities.count(); i++)
    {
        qDebug() << "Date" << activities[i]->getDate();
        for(int j = 0; j < activities[i]->count(); j++)
        {
            qDebug() << activities[i]->activity(j)->getUrl() << activities[i]->activity(j)->getDate();
        }
    }
    */
}
