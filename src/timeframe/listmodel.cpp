#include "listmodel.h"
#include "listitem.h"
#include <QtCore/QDebug>

ListModel::ListModel(QHash<int, QByteArray> roles, QObject *parent) :
    QAbstractListModel(parent), m_roles(roles)
{
    setRoleNames(m_roles);
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);    
    return m_items.size();    
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{    
    if(index.row() < 0 || index.row() >= m_items.size())
        return QVariant();
    return m_items.at(index.row())->data(role);
}

ListModel::~ListModel()
{
    clear();
}

void ListModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem*>() << item);
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach(ListItem *item, items)
        m_items.append(item);
    endInsertRows();
}

void ListModel::insertRow(int row, ListItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

void ListModel::handleItemChange()
{
}

ListItem * ListModel::find(const QString &id) const
{
    foreach(ListItem* item, m_items)
        if(item->id() == id)
            return item;
    return 0;
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    Q_ASSERT(item);
    for(int row = 0; row < m_items.size(); ++row)
        if(m_items.at(row) == item)
            return index(row);

    return QModelIndex();
}

void ListModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void ListModel::show(int r)
{
    qDebug() << "show 100600";
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_items.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_items.takeAt(row);
    endRemoveRows();
    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || ( row + count) >= m_items.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
        delete m_items.takeAt(row);
    endRemoveRows();
    return true;
}

ListItem * ListModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ListItem* item = m_items.takeAt(row);
    endRemoveRows();
    return item;
}

