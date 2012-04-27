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
    hash.insert(CompletedRole, "complete");

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
    connect(proxy, SIGNAL(monthFinished(QDate)), SLOT(monthFinished(QDate)));
    connect(proxy, SIGNAL(newMonth(int,int)), SLOT(newMonth(int,int)));
}

void ActivityModel::newActivitySet(ActivitySet *set)
{

    QDate d = set->getDate();
    d.setDate(d.year(), d.month(), 1); // set day to 1 because we only care about year and month

    if (m_dateSet.contains(set->getDate().toString()))
    {
        return;
    }
    int ind = 0;
    bool insertIntoExisting = false;
    for( ; ind < m_list.size(); ind++)
    {        
        if((m_list[ind]->date() == d) && (m_list[ind]->count() < 3))
        {
            insertIntoExisting = true;
            break;
        }
        else if(m_list[ind]->date() < d)
            continue;
        else
            break;
    }    
    if(insertIntoExisting)
    {
        m_list[ind]->addSet(set);        
        emit dataChanged( index(ind, 0, QModelIndex()), index(ind, columnCount(QModelIndex()), QModelIndex()) );
    }
    else
    {
        ActivityList *list = new ActivityList(set->getDate().year(), set->getDate().month(), this);
        list->addSet(set);

        beginInsertRows(QModelIndex(), ind, ind);
        m_list.insert(ind, list);        
        endInsertRows();
    }
    m_dateSet.insert(set->getDate().toString());
}

void ActivityModel::newMonth(int year, int month)
{
    ActivityList *list = new ActivityList(year, month, this);
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

    beginInsertRows(QModelIndex(), index, index);
    m_list.insert(index, list);
    endInsertRows();
}

void ActivityModel::monthFinished(QDate date)
{    
    for(int i = 0; i < m_list.size(); i++)
    {
        if(m_list[i]->date() == date)
            m_list[i]->setComplete(true);
    }
}

void ActivityModel::isComplete()
{

    //!!!!!!!
    return;


    // dump activity lists on listing finished
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

QDate ActivityModel::getDateOfIndex(int listIndex)
{
    if (listIndex >= m_list.size() || listIndex < 0)
        return QDate();
    ActivityList* list = m_list.at(listIndex);    
    QDate date = list->date();
    return date;
}

bool ActivityModel::isNullItem(int listIndex)
{
    if (listIndex >= m_list.size())
        return false;
    ActivityList* list = m_list.at(listIndex);
    if (list->count())
        return false;
    return true;
}

int ActivityModel::getListIndex(int year, int month, bool direction)
{
    int ind;
    QDate date(year, month,1);
    if (direction) // forward
    {
        ind =0;
        for( ; ind < m_list.size(); ind++)
        {
            if(m_list[ind]->date() == date)
            {
                break;
            }
        }
    }
    else //backward
    {        
        ind = m_list.size()-1;
        for( ; ind >= 0; ind--)
        {
            if(m_list[ind]->date() == date)
            {
                break;
            }
        }        
    }
    return ind;
}

