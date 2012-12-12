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

#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMetaType>
#include <QtCore/QSet>
#include <QtCore/QDate>
#include <QtGui/QSortFilterProxyModel>

class Activity;

#pragma GCC diagnostic ignored "-Woverloaded-virtual"

/**
 * @brief The LocalDayModel class stores day's local contents
 *        and gives some helper members to be used from QML
 *        side
 */
class LocalDayModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole,
        UrlRole,
        TypeRole
    };
    explicit LocalDayModel(QDate date, QObject *parent = 0);
    ~LocalDayModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE QString url(int row) const;

    QDate date() const;
    void setDate(QDate date);

signals:
    void gotThumbnail(QString path);

public slots:
    void addActivityItem(Activity *item);

private:
    enum {
        ImageItemMax = 4,
        VideoItemMax = 2,
        DocumentItemMax = 1,
        AllItemMax = ImageItemMax + VideoItemMax + DocumentItemMax
    };

    /**
      * @brief LocalDayModel::getImageInsertPosition - return position of inserted
      *        image item, items must be sorted by descending of dimension image
      * @param item - item which add to model
      * @param from - start position in the list of items
      * @param to - end position in the list of items
      * @return -1 - need append item to the list
      *         pos - need insert item to the position pos in the list
      */
    int getImageInsertPosition(Activity *item, int from, int to) const;

    /**
     * @brief LocalDayModel::getInsertPosition - return position of
     *        inserted item, first 7 items of the list must be LocalDayModel::ImageItemMax images,
     *        LocalDayModel::VideoItemMax video, LocalDayModel::DocumentItemMax document
     * @param item - item which add to model
     * @return -1 - need append item to the list
     *         pos - need insert item to the position pos in the list
     */
    int getInsertPosition(Activity *item) const;
    void addActivityItemToCloud(Activity *item, int pos);
    void printFirstItems(int count);

private:
    QHash<int, QByteArray> m_hash;
    QList<Activity *> m_items;
    QSet<QString> m_urlSet;
    QHash<QString, QSize> m_imgDimensions;
    QDate m_date;

};

/**
 * @brief The LocalDayFilterModel class allows day's content
 *        kept in LocalDayModel to be filtered
 */
class LocalDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalDayFilterModel(QObject *parent = 0);
    void setSourceModel(LocalDayModel *sourceModel);

    Q_INVOKABLE int count() const;
    Q_INVOKABLE QString url(int row) const;
    void previewReady(const QString &url);

signals:
    void gotThumbnail(QString path);
};


Q_DECLARE_METATYPE(LocalDayModel *)

#endif // ITEMMODEL_H
