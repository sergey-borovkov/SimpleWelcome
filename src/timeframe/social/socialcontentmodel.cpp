/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeca@osinit.ru>
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "socialdayitem.h"
#include "socialcontentmodel.h"
#include "socialitem.h"
#include "socialdaymodel.h"
#include "socialproxy.h"

#include <QtCore/QRegExp>
#include <QtCore/QVariant>

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

QObject *SocialContentModel::itemsModel(QDate date) const
{
    SocialContentItem *item = findItemByDate(date);
    return item ? item->model() : 0;
}

void SocialContentModel::resetModel()
{
    m_idSet.clear();
    m_idHash.clear();
    removeRows(0, rowCount());
    clear();
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

void SocialContentModel::updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->updateAudioUrl(audioId, audioOwnerId, audioUrl, eventId);
}

void SocialContentModel::updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->updateVideoUrl(videoId, videoOwnerId, videoUrl, videoImage, eventId);
}

void SocialContentModel::updateUserInfo(const QString &name, const QString &url, const QString &eventId)
{
    QDate date = m_idHash.value(eventId);
    SocialContentItem *item = findItemByDate(date);
    if (item)
        item->updateUserInfo(name, url, eventId);
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
            QString uniqueId = item->data(SocialItem::PluginName).toString() + item->id();
            if (item->data(SocialItem::PluginName).toString() == type) {
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
    SocialContentItem *item = static_cast<SocialContentItem *>(sender());
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
        SocialItem *newItem = list.at(i);
        QString uniqueId = newItem->data(SocialItem::PluginName).toString() + newItem->id();
        if (m_idSet.contains(uniqueId))
            continue;
        m_idSet.insert(uniqueId);
        m_idHash.insert(newItem->id(), newItem->datetime().date());

        int j = 0;
        bool flag = false;
        if (rowCount() > 0) {
            SocialContentItem *item = static_cast<SocialContentItem *>(itemAt(j));
            while (item->date() <= newItem->datetime().date()) {
                if (item->date() == newItem->datetime().date()) {
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

        SocialContentItem *socialDayItem = new SocialContentItem(newItem->datetime().date());
        socialDayItem->addSocialItem(newItem);

        insertRow(j, socialDayItem);
    }
}


SocialContentFilterModel::SocialContentFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(SocialContentItem::ItemsTypes);
}

void SocialContentFilterModel::setFilter(const QString &filter)
{
    QRegExp filterRegExp;
    if (filter == QLatin1String("Social"))
        filterRegExp = QRegExp("Facebook|VKontakte|Twitter");
    else
        filterRegExp = QRegExp(filter);
    setFilterRegExp(filterRegExp);

    for (int i = 0; i < rowCount(); i++) { //Set filter on nested models
        QDate date = data(index(i, 0), SocialContentItem::DateRole).toDate();
        SocialDayFilterModel *sModel = qobject_cast<SocialDayFilterModel *> (itemsModel(date));
        if (sModel)
            sModel->setFilterRegExp(filterRegExp);
    }
}

void SocialContentFilterModel::resetModel()
{
    SocialContentModel *model = qobject_cast<SocialContentModel *>(sourceModel());
    if (model) {
        model->resetModel();
    }
}


QObject *SocialContentFilterModel::itemsModel(QDate date) const
{
    SocialContentModel *model = qobject_cast<SocialContentModel *>(sourceModel());
    if (model)
        return model->itemsModel(date);
    return 0;
}

int SocialContentFilterModel::getIndexByDate(int year, int month,  bool direction) const
{
    Q_UNUSED(direction)
    for (int i = 0; i < rowCount(); i++) {
        QDate date = data(index(i, 0), SocialContentItem::DateRole).toDate();
        if ((date.year() == year) && (date.month() == month))
            return i;
    }
    return -1;
}

int SocialContentFilterModel::getIndexByDate(QDate date)
{
    for (int i = 0; i < rowCount(); i++) {
        QDate contentDate = data(index(i, 0), SocialContentItem::DateRole).toDate();
        if (contentDate >= date)
            return i;
    }
    return rowCount() - 1 ;
}

int SocialContentFilterModel::count()
{
    return rowCount();
}

QDate SocialContentFilterModel::getDateOfIndex(int listIndex)
{
    if ((listIndex >= rowCount()) || (listIndex < 0))
        return QDate();
    return data(index(listIndex, 0), SocialContentItem::DateRole).toDate();
}
