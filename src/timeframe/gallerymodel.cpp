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

GalleryModel::~GalleryModel()
{
    clear();
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
    connect( m_lister, SIGNAL(newMonth(int,int)), this, SLOT(newMonth(int,int)));
}

void GalleryModel::newActivities(QList<Activity*> list)
{
    // qDebug() << "------new Activities--------";
    for (int i = 0; i < list.size() ; i++)
    {

        Activity* item = list.at(i);
        m_urlHash.insert(item->getUrl(),item->getDate());
        //qDebug() << item->getDate();

        //first check of null item, if we find one, edit him with new data
        foreach (GalleryItem* it, m_items)
        {
            if ( (it->getDate().year() == item->getDate().year())
                 && (it->getDate().month() == item->getDate().month())
                 && (it->getCount() == 0)
                 )
            {
                it->setDate(item->getDate());
                it->addActivity(item);
                QModelIndex index = indexFromItem(it);
                if (index.isValid())
                    dataChanged(index, index);
                continue;
            }
        }

        int j = 0;
        bool flag = false;
        if (m_items.size() > 0)
        {
            while (m_items.at(j)->getDate() <= item->getDate())
            {
                if ( m_items.at(j)->getDate() == item->getDate())
                {
                    m_items.at(j)->addActivity(item);
                    flag = true;
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
        // qDebug() << "new gallery item" << item->getDate();
        if (removeNullItem(item->getDate().year(), item->getDate().month()))
            j--;
        GalleryItem * gallItem = new GalleryItem(item->getDate());
        gallItem->model()->addActivityItem(item);
        insertRow(j,gallItem);
    }
}

//Add null gallery item to model
void GalleryModel::newMonth(int year, int month)
{
    QDate date(year, month, 1);
    foreach(GalleryItem* item, m_items)
    {
        if((item->getDate().year() == year) && (item->getDate().month() == month))
        {
            return;
        }
    }
    int j = 0;
    GalleryItem * gallItem = new GalleryItem(date);
    if (m_items.size() > 0)
    {
        while (m_items.at(j)->getDate() <= gallItem->getDate())
        {
            j++;
            if ( j == m_items.size() )
            {
                break;
            }
        }
    }
//    qDebug() << "new null item" << date;
    insertRow(j,gallItem);
}

//Remove null gallery item from model
bool GalleryModel::removeNullItem(int year, int month)
{
//    qDebug() << "call Remove null" << year << month;
    QDate date(year, month, 1);
    int j = 0;
    if (m_items.size() > 0)
    {
        while (m_items.at(j)->getDate() <= date)
        {
            if ( m_items.at(j)->getDate() == date)
            {
                if ( m_items.at(j)->model()->rowCount( QModelIndex( ) ) == 0 )
                {
//                    qDebug() << "Remove null" << year << month;
                    removeRow(j, QModelIndex());
                    return true;
                }
            }
            j++;
            if ( j == m_items.size() )
            {
                break;
            }
        }
    }
    return false;
}


void GalleryModel::appendRow(GalleryItem *item)
{
    appendRows(QList<GalleryItem*>() << item);
}

void GalleryModel::appendRows(const QList<GalleryItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(GalleryItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_items.append(item);
    }
    endInsertRows();
}

void GalleryModel::insertRow(int row, GalleryItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_items.insert(row, item);
    endInsertRows();
}

void GalleryModel::handleItemChange()
{
    GalleryItem* item = static_cast<GalleryItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

GalleryItem * GalleryModel::find(const QDate &date) const
{
    foreach(GalleryItem* item, m_items) {
        if(item->getDate() == date) return item;
    }
    return 0;
}

QModelIndex GalleryModel::indexFromItem(const GalleryItem *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_items.size(); ++row) {
        if(m_items.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void GalleryModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

bool GalleryModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_items.size()) return false;
    beginRemoveRows(QModelIndex(), row, row);
    /*delete*/ m_items.takeAt(row);
    endRemoveRows();
    return true;
}

bool GalleryModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || (row+count) >= m_items.size()) return false;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i<count; ++i) {
        m_items.takeAt(row)->deleteLater();
    }
    endRemoveRows();
    return true;
}

int GalleryModel::getIndexByDate(int year, int month, bool direction)
{
    //QDate date(year, month,1);
    //return indexFromItem(find(date)).row();
    GalleryItem* ptr=0;
    foreach(GalleryItem* item, m_items)
    {
        if((item->getDate().year() == year) && (item->getDate().month() == month))
        {
            ptr = item;
            break;
        }
    }
    if (ptr)
        return indexFromItem(ptr).row();
    return -1;

//    if (direction) // forward
//    {
//        ind =0;
//        for( ; ind < m_list.size(); ind++)
//        {
//            if(m_list[ind]->date() == date)
//            {
//                break;
//            }
//        }
//    }
//    else //backward
//    {
//        ind = m_list.size()-1;
//        for( ; ind >= 0; ind--)
//        {
//            if(m_list[ind]->date() == date)
//            {
//                break;
//            }
//        }
//    }
//    return ind;
}


QDate GalleryModel::getDateOfIndex(int listIndex)
{
    if (listIndex >= m_items.size() || listIndex < 0)
        return QDate();
    GalleryItem* item = m_items.at(listIndex);
    QDate date = item->getDate();
    return date;
}

void GalleryModel::imageReady(QString url)
{
    if (m_urlHash.contains(url))
    {
        QDate date = m_urlHash[url];
        GalleryItem* item = find(date);
        item->thumbnailReady(url);
    }
}
