#include "socialmodel.h"
#include "socialitem.h"

#include <QtCore/QDebug>

SocialModel::SocialModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roles.insert(SocialItem::Text, "message");
    m_roles.insert(SocialItem::Image, "picture");
    setRoleNames(m_roles);
}

int SocialModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

QVariant SocialModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return QVariant();
    return m_list.at(index.row())->data(role);
}

SocialModel::~SocialModel()
{
    clear();
}

void SocialModel::appendRow(SocialItem *item)
{
    appendRows(QList<SocialItem*>() << item);
}

void SocialModel::appendRows(const QList<SocialItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach(SocialItem *item, items)
        m_list.append(item);
    endInsertRows();
}

void SocialModel::insertRow(int row, SocialItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    m_list.insert(row, item);
    endInsertRows();
}

void SocialModel::handleItemChange()
{
}

SocialItem * SocialModel::find(const QString &id) const
{
    foreach(SocialItem* item, m_list)
        if(item->id() == id)
            return item;
    return 0;
}

QModelIndex SocialModel::indexFromItem(const SocialItem *item) const
{
    Q_ASSERT(item);
    for(int row = 0; row < m_list.size(); ++row)
        if(m_list.at(row) == item)
            return index(row);

    return QModelIndex();
}

void SocialModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

bool SocialModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

bool SocialModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || ( row + count) >= m_list.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
        delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

SocialItem * SocialModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    SocialItem* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}
