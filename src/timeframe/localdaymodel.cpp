#include "localdaymodel.h"
#include "activityset.h"
#include "localdayitem.h"
#include "activityproxy.h"
#include "itemmodel.h"

#include <QVariant>
#include <QRegExp>
#include <QSortFilterProxyModel>


TimeFrameDayFilterModel::TimeFrameDayFilterModel(QObject *parent) :
    QSortFilterProxyModel (parent)
{
    setDynamicSortFilter(true);
    setFilterRole(LocalDayModel::TypesRole);
}

void TimeFrameDayFilterModel::setFilter(const QString &filter)
{
    QRegExp filterRegExp;
    if (filter == "Local")
        filterRegExp = QRegExp("Image|Video|Doc");
    else if (filter == "Photo")
        filterRegExp = QRegExp("Image");
    else if (filter == "Video")
        filterRegExp = QRegExp("Video");
    else if (filter == "Documents")
        filterRegExp = QRegExp("Document");
    setFilterRegExp(filterRegExp);

    for (int i = 0; i < rowCount(); i++) //Set filter on nested models
    {
        QDate date = data(index(i,0),LocalDayModel::CurrentDateRole).toDate();
        TimeFrameFilterModel * tModel = qobject_cast<TimeFrameFilterModel *> (itemsModel(date));
        if (tModel)
            tModel->setFilterRegExp(filterRegExp);
    }


}

QObject* TimeFrameDayFilterModel::itemsModel(QDate date) const
{
    LocalDayModel* model = qobject_cast<LocalDayModel*>(sourceModel());
    if (model)
        return model->itemsModel(date);
    return 0;
}

QDate TimeFrameDayFilterModel::getDateOfIndex(int listIndex)
{
    if ((listIndex >= rowCount()) || (listIndex < 0))
        return QDate();
    return data(index(listIndex,0),LocalDayModel::CurrentDateRole).toDate();
}

int TimeFrameDayFilterModel::getIndexByDate(int year, int month,  bool direction)
{
    Q_UNUSED(direction)
    for (int i = 0; i < rowCount(); i++)
    {
        QDate date = data(index(i,0),LocalDayModel::CurrentDateRole).toDate();
        if((date.year() == year) && (date.month() == month))
                return i;
    }
    return -1;
}


LocalDayModel::LocalDayModel(QObject *parent) :
    QAbstractListModel(parent),
    m_lister(0)

{
    hash.insert(ItemsRole, "items");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(TypesRole, "type");
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

LocalDayModel::~LocalDayModel()
{
    clear();
}

QVariant LocalDayModel::data(const QModelIndex &index, int role) const
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
        //QVariant v;
        //v.setValue( m_items[index.row()]->model());
        return 0;//m_items.value(index.row());
    }
    else if(role == CountRole)
    {
        return m_items.size();
    }
    else if(role == TypesRole)
    {
        return m_items.at(index.row())->types();
    }
    else if(role == ItemsCountRole)
    {
        return m_items[index.row()]->getCount();
    }
    return QVariant();
}

int LocalDayModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_items.size();
}

QObject* LocalDayModel::itemsModel(QDate date) const
{
    for (int i = 0; i < m_items.size(); i++)
    {
        if (date == m_items.value(i)->getDate())
            return m_items.value(i)->model();
    }
    return 0;
}

void LocalDayModel::setLister(ActivityProxy *lister)
{
    if (m_lister)
        delete m_lister;
    m_lister = lister;
    m_lister->setModel(this);
    connect( m_lister, SIGNAL(newActivities(QList<Activity*>)), this, SLOT(newActivities(QList<Activity*>)) );
    connect( m_lister, SIGNAL(changeFilterString(QString)), this, SLOT(setActivityType(QString)));
}

void LocalDayModel::newActivities(QList<Activity*> list)
{
    // qDebug() << "------new Activities--------";
    for (int i = 0; i < list.size() ; i++)
    {

        Activity* item = list.at(i);
        if (m_urlHash.contains(item->getUrl()))
                continue;
        m_urlHash.insert(item->getUrl(),item->getDate());
        //qDebug() << item->getDate();

        //first check of null item, if we find one, edit him with new data
        foreach (LocalDayItem* it, m_items)
        {
            if ( (it->getDate().year() == item->getDate().year())
                 && (it->getDate().month() == item->getDate().month())
                 && (it->getCount() == 0)
                 )
            {
                it->setDate(item->getDate());
                it->addActivity(item);
                //qDebug() << "add activity into null object" << item->getDate();
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
//        int j = m_items.size()-1;
//        bool flag = false;
//        if (m_items.size() > 0)
//        {
//            while (m_items.at(j)->getDate() <= item->getDate())
//            {
//                if ( m_items.at(j)->getDate() == item->getDate())
//                {
//                    m_items.at(j)->addActivity(item);
//                    //qDebug("add activity into existing");
//                    flag = true;
//                    break;
//                }
//                j--;
//                if ( j == 0 )
//                {
//                    break;
//                }
//            }
//        }

        if (flag)
        {
            continue;
        }

        //qDebug() << "new gallery item" << item->getDate();
        //if (removeNullItem(item->getDate().year(), item->getDate().month()))
          //  j--;

        LocalDayItem * gallItem = new LocalDayItem(item->getDate());
        gallItem->addActivity(item);
        insertRow(j,gallItem);

        //insertRow(j+1,gallItem);
        //qDebug() <<" add new activity" << item->getDate();
    }
}

//Add null gallery item to model
void LocalDayModel::newMonth(int year, int month)
{
    QDate date(year, month, 1);
    foreach(LocalDayItem* item, m_items)
    {
        if((item->getDate().year() == year) && (item->getDate().month() == month))
        {
            return;
        }
    }
    int j = 0;
    LocalDayItem * gallItem = new LocalDayItem(date);
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
//    GalleryItem * gallItem = new GalleryItem(date);
//    if (m_items.size() > 0)
//    {
//        qDebug() << m_items.at(j)->getDate() << gallItem->getDate();
//        while (m_items.at(j)->getDate() <= gallItem->getDate())
//        {
//            qDebug() << "j--";
//            j--;
//            if ( j == 0 )
//            {
//                break;
//            }
//        }
//    }
//    qDebug() << "new null item" << date;
    insertRow(j,gallItem);
    //insertRow(j+1,gallItem);
}

//Remove null gallery item from model
bool LocalDayModel::removeNullItem(int year, int month)
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


void LocalDayModel::appendRow(LocalDayItem *item)
{
    appendRows(QList<LocalDayItem*>() << item);
}

void LocalDayModel::appendRows(const QList<LocalDayItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(LocalDayItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_items.append(item);
    }
    endInsertRows();
}

void LocalDayModel::insertRow(int row, LocalDayItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_items.insert(row, item);
    endInsertRows();
}

void LocalDayModel::handleItemChange()
{
    LocalDayItem* item = static_cast<LocalDayItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

LocalDayItem * LocalDayModel::find(const QDate &date) const
{
    foreach(LocalDayItem* item, m_items) {
        if(item->getDate() == date) return item;
    }
    return 0;
}

QModelIndex LocalDayModel::indexFromItem(const LocalDayItem *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_items.size(); ++row) {
        if(m_items.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void LocalDayModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

bool LocalDayModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_items.size()) return false;
    beginRemoveRows(QModelIndex(), row, row);
    /*delete*/ m_items.takeAt(row);
    endRemoveRows();
    return true;
}

bool LocalDayModel::removeRows(int row, int count, const QModelIndex &parent)
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

int LocalDayModel::getIndexByDate(int year, int month,  bool direction)
{
    Q_UNUSED(direction)
    //QDate date(year, month, day);
    //return indexFromItem(find(date)).row();

    LocalDayItem* ptr=0;

//    for (int i = m_items.size() -1; i >= 0; i--)
//    {
//        if((m_items.at(i)->getDate().year() == year) && (m_items.at(i)->getDate().month() == month))
//        {
//            ptr = m_items.at(i);
//            break;
//        }
//    }

    foreach(LocalDayItem* item, m_items)
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
}


QDate LocalDayModel::getDateOfIndex(int listIndex)
{
    if (listIndex >= m_items.size() || listIndex < 0)
        return QDate();
    LocalDayItem* item = m_items.at(listIndex);
    QDate date = item->getDate();
    return date;
}

void LocalDayModel::imageReady(QString url)
{
    if (m_urlHash.contains(url))
    {
        QDate date = m_urlHash[url];
        LocalDayItem* item = find(date);
        if (item)
            item->thumbnailReady(url);
    }
}
