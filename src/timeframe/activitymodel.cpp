#include "activitymodel.h"
#include "activityproxy.h"
#include "activitylist.h"
#include "activityset.h"

#include <QDebug>

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractItemModel(parent), dummy(0)
{

    hash.insert(ActivitiesRole, "activity");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(DummyRole, "dummy");

    setRoleNames(hash);
}

ActivityModel::~ActivityModel()
{
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_list.size())
        return QVariant();

    if(role == CurrentDateRole)
        return m_list[row]->date();
    else if(role == ActivitiesRole)
    {
        QVariant v;
        v.setValue( m_list[row] );
        return v;
    }
    else if(role == CountRole)
        return m_list[row]->date();
    else if(role == DummyRole)
        return dummy;
    return QVariant();
}

int ActivityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.size();
}

void ActivityModel::addProxy(ActivityProxy *proxy)
{
    connect(proxy, SIGNAL(newActivitySet(ActivitySet*)), SLOT(newActivitySet(ActivitySet*)));
    /*
    connect(proxy, SIGNAL(newMonth(int,int)), SLOT(newMonth(int,int)));
    */
}

void ActivityModel::newActivitySet(ActivitySet *set)
{    
    QDate d = set->getDate();
    d.setDate(d.year(), d.month(), 1); // set day to 1 because we only care about year and month

    bool insertIntoExisting = false;
    ActivityList *list;
    int i = 0;

    foreach(list, m_list)
    {
        i++;
        if(list->date() == d)
        {
            for(int i = 0; i < list->count(); i++)
                if(list->at(i)->getDate() == set->getDate()) // duplicate
                    return;

            if(list->count() < 3)
            {
                insertIntoExisting = true;
                break;
            }
        }
        else if(list->date() > d)
            break;
    }

    if(insertIntoExisting)
    {
       list->addSet(set);
       emit dataChanged( index(i, 0, QModelIndex()), index(i, columnCount(QModelIndex()), QModelIndex()) );
       emit rowChanged(i);
    }
    else // all activity lists are full or no activity list for set's year and month
    {
        ActivityList *list = new ActivityList(set->getDate().year(), set->getDate().month(), this);
        list->addSet(set);

        beginInsertRows(QModelIndex(), i, i + 1);
        m_list.insert(i, list);
        endInsertRows();
        emit newRow(i);
    }    
}

void ActivityModel::newMonth(int year, int month)
{
    ActivityList *list = new ActivityList(year, month);
    int index = 0;

    for( ; index < m_list.size(); index++)
    {
        QDate d = m_list[index]->date();
        d.setDate(d.year(), d.month(), 1);

        if(d == list->date())
            return;
        else if(d < list->date())
            continue;
        else
            break;
    }

    beginInsertRows(QModelIndex(), index, index + 1);
    m_list.insert(index, list);
    endInsertRows();
}

