#include "itemmodel.h"
#include "activityset.h"
#include <QDate>
#include <QDebug>

ItemModel::ItemModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_hash.insert(ActivitiesRole, "activity");
    m_hash.insert(CurrentDateRole, "date");
    m_hash.insert(CountRole, "count");
    m_hash.insert(UrlRole, "url");
    m_hash.insert(TypeRole, "type");
    setRoleNames(m_hash);    
}

ItemModel::~ItemModel()
{
    qDebug() << "delete items model";
    qDeleteAll(m_items);
    m_items.clear();
    qDebug() << "after delete items model";
}


QVariant ItemModel::data(const QModelIndex &index, int role) const
{    
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == CurrentDateRole)
    {
        return m_items.value(index.row())->getDate();
    }
    else if(role == ActivitiesRole)
    {
        QVariant v;
        v.setValue( m_items[index.row()]);
        return v;
    }
    else if(role == CountRole)
    {
        return m_items.size();
    }
    else if(role == UrlRole)
    {
        return m_items[index.row()]->getUrl();
    }
    else if(role == TypeRole)
    {
        return m_items[index.row()]->getType();
    }
    return QVariant();
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_items.size();
}

void ItemModel::addActivityItem(Activity *item)
{
    if (!item)
        return;    
    /* Check dublicates*/
    if (m_urlSet.contains(item->getUrl()))
        return;
    int ind = m_items.size();
    //qDebug() << ind;
    beginInsertRows(QModelIndex(), ind , ind);
    m_items.append(item);
    m_urlSet.insert(item->getUrl());
    endInsertRows();
}
