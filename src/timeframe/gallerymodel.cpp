#include "gallerymodel.h"
#include "activityset.h"
#include "galleryitem.h"
#include "itemmodel.h"

#include <QVariant>

GalleryModel::GalleryModel(QObject *parent) :
    QAbstractListModel(parent)
{
    hash.insert(ItemsRole, "items");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(ItemsCountRole, "size");
    setRoleNames(hash);
    beginInsertRows(QModelIndex(), 0 , 4);
    for (int i = 0; i < 5; i++)
    {
        GalleryItem * item = new GalleryItem(QDate(2012,05,i+1));
        m_items.append(item);
    }
    endInsertRows();
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

void GalleryModel::newActivity(Activity *item)
{
    /*TO-DO: add new Gallery items in model */

    /*int row = m_items.size();
    beginInsertRows(QModelIndex(), row , row);
    m_items.append(item);
    endInsertRows();
    */
}


int GalleryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_items.size();
}

QObject* GalleryModel::itemsModel(int index) const
{
    GalleryItem *item = m_items.value(index);
    if(item) {
        return  item->model();
    }
    return 0;
}
