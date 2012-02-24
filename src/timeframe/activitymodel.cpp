#include "activitymodel.h"
#include "activitysource.h"

#include <QDate>
#include <QDebug>

#include "activityset.h"

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent)
{
    currentDate = QDate::currentDate();
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
    source->startSearch(QDate::currentDate().addDays(-3), QDate::currentDate());
    connect(source, SIGNAL(newActivitySet(ActivitySet*)), SLOT(addActivitySet(ActivitySet*)));
}

void ActivityModel::addActivitySet(ActivitySet *set)
{
    // just to get it working
    for(int i = 0; i < set->count(); i++)
    {

            for(int j = 0; j < activities.size(); j++)
            {
                bool isFound = false;
                for(int k = 0; k < activities[j]->count(); k++)
                {
                    if(activities[j]->activity(k)->getDate() == set->activity(i)->getDate())
                    {
                        activities[j]->addActivity(set->activity(i));
                        isFound = true;
                        break;
                    }

                    if(!isFound)
                    {
                        ActivitySet *newSet = new ActivitySet;
                        newSet->addActivity(set->activity(i));
                        activities.append(newSet);
                    }
                }
            }

    }
/*
    activities.append(set);
*/

}
