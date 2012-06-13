#include "gallerymodel.h"
#include "activityset.h"
#include "galleryitem.h"
#include "gallerylister.h"
#include "itemmodel.h"

#include <QVariant>

GalleryModel::GalleryModel(QObject *parent) :
    QAbstractListModel(parent),
    m_lister(0)

{
    hash.insert(ItemsRole, "items");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(ItemsCountRole, "size");
    setRoleNames(hash);
    /*
    beginInsertRows(QModelIndex(), 0 , 4);
    for (int i = 0; i < 5; i++)
    {
        GalleryItem * item = new GalleryItem(QDate(2012,05,i+1));
        m_items.append(item);
    }
    endInsertRows();
    */
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{    
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == CurrentDateRole)
    {
        return m_items.value(index.row())->getDate();
    }
    else if(role == ItemsRole)
    {
        QVariant v;
        v.setValue( m_items[index.row()]->model());
        return v;//m_items.value(index.row());
    }
    else if(role == CountRole)
    {
        return m_items.size();
    }
    else if(role == ItemsCountRole)
    {
        return m_items[index.row()]->getCount();
    }
    return QVariant();
}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_items.size();
}

QObject* GalleryModel::itemsModel(QDate date) const
{    
    for (int i = 0; i < m_items.size(); i++)
    {
        if (date == m_items.value(i)->getDate())
            return m_items.value(i)->model();
    }
    return 0;
}

void GalleryModel::setLister(GalleryLister *lister)
{
    if (m_lister)
        delete m_lister;
    m_lister = lister;
    connect( m_lister, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newActivities(QList<Activity*>)) );
}

void GalleryModel::newActivities(QList<Activity*> list)
{
   // qDebug() << "------new Activities--------";
    for (int i = 0; i < list.size() ; i++)
    {
        Activity* item = list.at(i);
        //qDebug() << item->getDate();
        int j = 0;
        bool flag = false;
        if (m_items.size() > 0)
        {
            //qDebug() << " try to enter WHILE" <<  m_items.at(j)->getDate() << item->getDate();
            while (m_items.at(j)->getDate() <= item->getDate())
            {
                if ( m_items.at(j)->getDate() == item->getDate())
                {

                    m_items.at(j)->model()->newItem(item);
                    emit dataChanged( index(j), index(j));
                    flag = true;
                    //qDebug() << "append into existing day";
                    break;
                }
                j++;
                if ( j == m_items.size() )
                {
                    break;
                }
            }
        }
        if (flag)
        {
            continue;
        }
        //qDebug() << "new gallery item";
        GalleryItem * gallItem = new GalleryItem(item->getDate());
        gallItem->model()->newItem(item);
        beginInsertRows(QModelIndex(), j , j);
        m_items.insert(j, gallItem);
        endInsertRows();
    }
    //qDebug() << "-------------------------" ;
}

