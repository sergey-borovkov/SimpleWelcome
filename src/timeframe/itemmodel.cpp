#include "itemmodel.h"
#include "activityset.h"
#include "QDate"

ItemModel::ItemModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_hash.insert(ActivitiesRole, "activity");
    m_hash.insert(CurrentDateRole, "date");
    m_hash.insert(CountRole, "count");
    m_hash.insert(UrlRole, "url");
    m_hash.insert(TypeRole, "type");
    setRoleNames(m_hash);
    /* T0-DO: connect wint nepomuk source */
    beginInsertRows(QModelIndex(), 0 , 4);
    for (int i = 0; i < 5; i++)
    {
        Activity * item = new Activity(QString("/home/saint/images/%1.jpg").arg(i+1), "image", QDate(2012,5,1));
        m_items.append(item);
    }
    endInsertRows();
    /* */
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
