#include "socialdayitem.h"
#include "socialcontentmodel.h"
#include "socialitem.h"
#include "socialitemmodel.h"
#include "socialproxy.h"

#include <QtCore/QRegExp>
#include <QtCore/QVariant>

SocialDayFilterModel::SocialDayFilterModel(QObject * parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(SocialContentItem::ItemsTypes);
}

void SocialDayFilterModel::setFilter(const QString &filter)
{
    QRegExp filterRegExp;
    if (filter == "Social")
        filterRegExp = QRegExp("Facebook|VKontakte|Twitter");
    else
        filterRegExp = QRegExp(filter);
    setFilterRegExp(filterRegExp);

    for (int i = 0; i < rowCount(); i++) { //Set filter on nested models
        QDate date = data(index(i, 0), SocialContentItem::DateRole).toDate();
        SocialItemFilterModel * sModel = qobject_cast<SocialItemFilterModel *> (itemsModel(date));
        if (sModel)
            sModel->setFilterRegExp(filterRegExp);
    }
}

QObject* SocialDayFilterModel::itemsModel(QDate date) const
{
    SocialContentModel* model = qobject_cast<SocialContentModel*>(sourceModel());
    if (model)
        return model->itemsModel(date);
    return 0;
}

int SocialDayFilterModel::getIndexByDate(int year, int month,  bool direction)
{
    Q_UNUSED(direction)
    for (int i = 0; i < rowCount(); i++) {
        QDate date = data(index(i, 0), SocialContentItem::DateRole).toDate();
        if ((date.year() == year) && (date.month() == month))
            return i;
    }
    return -1;
}

QDate SocialDayFilterModel::getDateOfIndex(int listIndex)
{
    if ((listIndex >= rowCount()) || (listIndex < 0))
        return QDate();
    return data(index(listIndex, 0), SocialContentItem::DateRole).toDate();
}

SocialContentModel::SocialContentModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

void SocialContentModel::appendRows(const QList<SocialContentItem *> &items)
{
    QList<ListItem *> list;
    foreach(SocialContentItem * item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        list.append(item);
    }
    ListModel::appendRows(list);
}

void SocialContentModel::insertRow(int row, SocialContentItem *item)
{
    ListModel::insertRow(row, item);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
}

QObject* SocialContentModel::itemsModel(QDate date) const
{
    SocialContentItem *item = findItemByDate(date);
    return item ? item->model() : 0;
}

void SocialContentModel::likeItem(QString eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->likeItem(eventId);
}

void SocialContentModel::addCommentToItem(CommentItem *commentItem, QString eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->addCommentToItem(commentItem, eventId);
}

void SocialContentModel::updateUserImage(const QString &userId, const QString &userImageUrl, const QString &eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->updateUserImage(userId, userImageUrl, eventId);
}

void SocialContentModel::updateUserName(const QString &userId, const QString &userName, const QString &eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->updateUserName(userId, userName, eventId);
}

void SocialContentModel::addComments(QString id, QList<CommentItem *> list)
{
    QDate date = m_idHash.value(id);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->addComments(id, list);
}

void SocialContentModel::setSelfLiked(QString id)
{
    QDate date = m_idHash.value(id);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->setSelfLiked(id);
}


void SocialContentModel::removeItems(const QString &type)
{
    for (int i = 0; i < rowCount(); i++) {
        SocialDayModel *model = static_cast<SocialContentItem *>(itemAt(i))->m_itemModel;
        for (int j = 0; j < model->rowCount(); j++) {
            SocialItem *item = static_cast<SocialItem *>(model->itemAt(j));
            QString uniqueId = item->pluginName() + item->id();
            if (item->pluginName() == type) {
                model->removeRow(j);
                m_idSet.remove(uniqueId);
                m_idHash.remove(uniqueId);
                j--;
            }
        }
        if (!model->rowCount()) {
            removeRow(i);
            i--;
        }
    }
}

void SocialContentModel::handleItemChange()
{
    SocialContentItem* item = static_cast<SocialContentItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if (index.isValid())
        emit dataChanged(index, index);
}

SocialContentItem *SocialContentModel::findItemByDate(const QDate &date) const
{
    int size = rowCount();
    for (int i = 0; i < size; i++) {
        SocialContentItem *item = static_cast<SocialContentItem *>(itemAt(i));
        if (date == item->date())
            return item;
    }
    return 0;
}

void SocialContentModel::newSocialItems(QList < SocialItem * > list)
{
    for (int i = 0; i < list.size() ; i++) {
        SocialItem* newItem = list.at(i);
        QString uniqueId = newItem->pluginName() + newItem->id();
        if (m_idSet.contains(uniqueId))
            continue;
        m_idSet.insert(uniqueId);
        m_idHash.insert(newItem->id(), newItem->date());

        int j = 0;
        bool flag = false;
        if (rowCount() > 0) {
            SocialContentItem *item = static_cast<SocialContentItem *>(itemAt(j));
            while (item->date() <= newItem->date()) {
                if (item->date() == newItem->date()) {
                    item->addSocialItem(newItem);
                    flag = true;
                    break;
                }
                j++;
                if (j == rowCount()) {
                    break;
                }
                item = static_cast<SocialContentItem *>(itemAt(j));
            }
        }

        if (flag) {
            continue;
        }

        SocialContentItem * socialDayItem = new SocialContentItem(newItem->date());
        socialDayItem->addSocialItem(newItem);

        insertRow(j, socialDayItem);
    }
}
