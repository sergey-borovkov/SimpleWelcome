#include "activitymodel.h"
#include "activitysource.h"
#include "activityset.h"

#include <QDate>
#include <QList>
#include <QDebug>
#include <QtAlgorithms>


uint qHash(const QDate &date)
{
    return qHash(date.toString());
}

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent)
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

int ActivityModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
    Q_UNUSED(parent)
    return activities.length();
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() > 0 && index.row() < rowCount())
    {
        int row = index.row();

        if ( role == CurrentDateRole )
        {
            return activities[index.row()].first;
        }
        else if ( role == ActivitiesRole )
        {
            ActivitySet *set = new ActivitySet(activities[row].second, 0);

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
    connect(this, SIGNAL(newSearch(QDate)), source, SLOT(startSearch(QDate)));
    connect(source, SIGNAL(finishedListing()), SLOT(listingFinished()));

}

void ActivityModel::addActivities(QList<Activity *> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        int idx = 0;
        bool isFound = false;

        while(idx < activities.count() && activities[idx].first <= list[i]->getDate())
        {
            if(activities[idx].first == list[i]->getDate())
            {
                isFound = true;
                break;
            }

            idx++;
        }

        if(isFound)
        {
            activities[idx].second.append(list[i]);
            emit dataChanged(index(idx), index(idx));
        }
        else
        {
            beginInsertRows(QModelIndex(), idx, idx + 1);
            activities.insert( idx, qMakePair(list[i]->getDate(), QList<Activity *>() << list[i]));
            endInsertRows();

            ActivitySet *set = new ActivitySet(activities[idx].second, 0);
            emit newSet(idx, set);
        }
    }
}

void ActivityModel::setMonth(int year, int month)
{
    QDate date;
    date.setDate(year, month + 1, 1);
    date.setDate(date.year(), date.month(), date.daysInMonth());

    if(date > QDate::currentDate())
        date = QDate::currentDate();

    emit newSearch(date);
}

int ActivityModel::getDateIndex(int year, int month)
{

    QDate date;
    date.setDate(year, month + 1, 1);

    date.setDate(date.year(), date.month(), date.daysInMonth());

    int idx = 0;
    bool isFound = false;

    while(idx < activities.count() && activities[idx].first < date)
    {
        idx++;
    }

    qDebug() << activities;

    return idx;
}

void ActivityModel::listingFinished()
{

}
