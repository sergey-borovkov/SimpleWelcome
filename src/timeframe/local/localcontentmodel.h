/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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

#ifndef LOCALCONTENTMODEL_H
#define LOCALCONTENTMODEL_H

#include <QtCore/QObject>
#include <QtGui/QSortFilterProxyModel>

#include <listmodel.h>
#include <QDate>

class LocalDayModel;
class Activity;
class ActivityProxy;
class LocalContentItem;

/**
 * @brief The LocalContentModel class stores local content
 *        by date, where every item keeps all content for
 *        certain day
 */
class LocalContentModel : public ListModel
{
    Q_OBJECT
public:
    explicit LocalContentModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(LocalContentItem *item);
    void appendRows(const QList<LocalContentItem *> &items);
    void insertRow(int row, LocalContentItem *item);
    void setLister(ActivityProxy *lister);
    void resetModel();
    LocalContentItem *find(const QDate &date) const;

    QRegExp filter() const;
    void setFilter(QRegExp regexp);

    void previewReady(const QString &path);

public slots:
    void newActivities(QList <Activity *> list);
    QObject *itemsModel(QDate date) const;
    int getIndexByDate(int year, int month, bool direction);
    QDate getDateOfIndex(int listIndex);

protected:
    bool removeNullItem(int, int);

private slots:
    void newMonth(int, int);
    void handleItemChange();

private:
    QHash<QString, LocalContentItem *> m_urlHash;
    ActivityProxy *m_lister;
    QRegExp m_filter;
};

/**
 * @brief The LocalContentFilterModel class allows filtering
 *        of LocalContentModel by content type - video/documents/images
 *        or everthing togethere
 */
class LocalContentFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalContentFilterModel(QObject *parent = 0);
    Q_INVOKABLE void setFilter(const QString &filter);
    Q_INVOKABLE QObject *itemsModel(QDate date) const;
    Q_INVOKABLE int getIndexByDate(int year, int month, bool direction);
    Q_INVOKABLE int getIndexByDate(QDate date);
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
    Q_INVOKABLE void resetModel();
    Q_INVOKABLE int count();
};

#endif // LOCALCONTENTMODEL_H
