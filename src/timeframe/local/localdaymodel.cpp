/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
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

#include "activity.h"
#include "localdaymodel.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>


LocalDayModel::LocalDayModel(QDate date, QObject *parent) :
    QAbstractListModel(parent), m_date(date)
{
    m_hash.insert(ActivitiesRole, "activity");
    m_hash.insert(CurrentDateRole, "date");
    m_hash.insert(CountRole, "count");
    m_hash.insert(UrlRole, "url");
    m_hash.insert(TypeRole, "type");
    setRoleNames(m_hash);
}

LocalDayModel::~LocalDayModel()
{
    qDeleteAll(m_items);
    m_items.clear();
}

QVariant LocalDayModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == CurrentDateRole) {
        return m_items.value(index.row())->date();
    } else if (role == ActivitiesRole) {
        QVariant v;
        v.setValue(m_items[index.row()]);
        return v;
    } else if (role == CountRole) {
        return m_items.size();
    } else if (role == UrlRole) {
        return m_items[index.row()]->url();
    } else if (role == TypeRole) {
        return m_items[index.row()]->type();
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

int LocalDayModel::getImageInsertPosition(Activity *item, int from, int to) const
{
    if (item->type() != "Image")
        return -1;
    if ((from < 0) && (to > m_items.size()))
        return -1;

    QSize szItem = item->imageSize();
    int squareItem = szItem.width() * szItem.height();

    bool isItemInserted = false;

    int row;
    for (row = from; row < to; ++row) {
        if (m_items.at(row)->type() != "Image")
            continue;

        QSize sz = m_items.at(row)->imageSize();
        int square = sz.width() * sz.height();

        if (squareItem > square) {
            isItemInserted = true;
            break;
        }
    }

    if (!isItemInserted)
        return -1;

    return row;
}

int LocalDayModel::getInsertPosition(Activity *item) const
{
    int cloudItemCount = m_items.size();
    if (cloudItemCount > AllItemMax) // max item's count in the cloud
        cloudItemCount = AllItemMax;

    int pos = -1; // if -1 - append item to end
    int imageCount, videoCount, docCount; // counters of first AllItemMax type items
    int imageRow, videoRow, docRow; // indexes of last type items

    QString typeItem = item->type();

    // count items diffrenet types (maybe count when add items???)
    imageCount = videoCount = docCount = 0;
    imageRow = videoRow = docRow = (-1); // indexes of last type items
    for (int row = 0; row < cloudItemCount; ++row) {

        if (m_items.at(row)->type() == "Image") {
            imageCount++;
            imageRow = row;
        }
        else if (m_items.at(row)->type() == "Video") {
            videoCount++;
            videoRow = row;
        }
        else {
            docCount++;
            docRow = row;
        }
    }

    if (cloudItemCount < AllItemMax) {
        if (typeItem == "Image")
            pos = getImageInsertPosition(item, 0, cloudItemCount);
        else
            pos = -1;
    }
    else {
        if (typeItem == "Image") {
            pos = getImageInsertPosition(item, 0, cloudItemCount);
            if (imageCount > ImageItemMax) { // get position for "cloud" items (first AllItemMax items)
                if (pos == (-1)) { // position not found - item need append to end
                    pos = getImageInsertPosition(item, 0, m_items.size());
                }
            }
            else {
                if (videoCount > VideoItemMax)
                    pos = videoRow;
                if (docCount > DocumentItemMax)
                    pos = docRow;
            }
        }

        if (typeItem == "Video") {
            if (videoCount > VideoItemMax) {
                pos = (-1);
            }
            else {
                if (imageCount > ImageItemMax)
                    pos = imageRow;
                if (docCount > DocumentItemMax)
                    pos = docRow;
            }
        }

        if (typeItem == "Document") {
            if (docCount > DocumentItemMax) {
                pos = (-1);
            }
            else {
                if (imageCount > ImageItemMax)
                    pos = imageRow;
                if (videoCount > VideoItemMax)
                    pos = videoRow;
            }
        }
    }

    return pos;
}

void LocalDayModel::addActivityItemToCloud(Activity *item, int pos)
{
    int cloudItemCount = m_items.size();
    if (cloudItemCount > AllItemMax) // max item's count in the cloud
        cloudItemCount = AllItemMax;

    QString typeItem = item->type();

    // count items diffrenet types (maybe count when add items???)
    int lastRow = (-1);
    for (int row = 0; row < cloudItemCount; ++row) {
        if (m_items.at(row)->type() == typeItem) {
            lastRow = row;
        }
    }

    if (lastRow == (-1))
        m_items.insert(cloudItemCount, item);
    else {
        m_items.move(lastRow, cloudItemCount);
        m_items.insert(pos, item);
    }
}

void LocalDayModel::addActivityItem(Activity *item)
{
    if (!item)
        return;

    /* Check duplicates*/
    if (m_urlSet.contains(item->url()))
        return;

    int ind = m_items.size();
    beginInsertRows(QModelIndex(), ind , ind);

    int pos = getInsertPosition(item);
    if (pos == (-1))
        m_items.append(item);
    else {
        if (pos < AllItemMax) { // switch items
            m_items.insert(AllItemMax, m_items.at(pos));
            m_items.replace(pos, item);
        }
        else
            m_items.insert(pos, item);
    }

    m_urlSet.insert(item->url());
    endInsertRows();
}

QString LocalDayModel::url(int row) const
{
    return m_items.at(row)->url();
}

QDate LocalDayModel::date() const
{
    return m_date;
}

void LocalDayModel::setDate(QDate date)
{
    m_date = date;
}


LocalDayFilterModel::LocalDayFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void LocalDayFilterModel::setSourceModel(LocalDayModel *sourceModel)
{
    if (sourceModel) {
        QSortFilterProxyModel::setSourceModel(sourceModel);
    }
}

int LocalDayFilterModel::count() const
{
    return rowCount();
}

QString LocalDayFilterModel::url(int row) const
{
    return data(index(row, 0), LocalDayModel::UrlRole).toString();
}

QString LocalDayFilterModel::type(int row) const
{
    return data(index(row, 0), LocalDayModel::TypeRole).toString();
}

void LocalDayFilterModel::previewReady(const QString &url)
{
    emit gotThumbnail(url);
}

