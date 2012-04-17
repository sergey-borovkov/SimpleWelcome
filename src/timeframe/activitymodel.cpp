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
    hash.insert(CompletedRole, "");

    setRoleNames(hash);
}

ActivityModel::~ActivityModel()
{
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
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
    connect(proxy, SIGNAL(finished()), SLOT(isComplete()));
    /*
    connect(proxy, SIGNAL(newMonth(int,int)), SLOT(newMonth(int,int)));
    */
}

void ActivityModel::newActivitySet(ActivitySet *set)
{
    QDate d = set->getDate();
    d.setDate(d.year(), d.month(), 1); // set day to 1 because we only care about year and month

    int ind = 0;
    bool insertIntoExisting = false;
    for( ; ind < m_list.size(); ind++)
        if(m_list[ind]->date() == d && m_list[ind]->count() < 3)
        {
            insertIntoExisting = true;
            break;
        }
        else if(m_list[ind]->date() < d)
            continue;
        else
            break;

    if(insertIntoExisting)
    {
        m_list[ind]->addSet(set);
        emit dataChanged( index(ind, 0, QModelIndex()), index(ind, columnCount(QModelIndex()), QModelIndex()) );
    }
    else
    {
        ActivityList *list = new ActivityList(set->getDate().year(), set->getDate().month(), this);
        list->addSet(set);

        beginInsertRows(QModelIndex(), ind, ind + 1);
        m_list.insert(ind, list);
        endInsertRows();
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

void ActivityModel::isComplete()
{
    for( int i = 0; i < m_list.size() ; i++ )
    {
        qDebug() << "list" << i;
        for(int j = 0; j < m_list[i]->count(); j++)
        {
            qDebug() << "set" << j;
            for(int k = 0; k < m_list[i]->getSetCount(j); k++)
            {
                qDebug() << m_list[i]->getUrl(j, k) << m_list[i]->getSetDate(j);
            }
        }
    }
}

