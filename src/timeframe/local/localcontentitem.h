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

#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtGui/QSortFilterProxyModel>

#include <listitem.h>

class Activity;
class QDate;
class LocalDayModel;
class LocalDayFilterModel;

/**
 * @brief The LocalContentItem class stores local content in one day
 */
class LocalContentItem : public QObject, public ListItem
{
    Q_OBJECT
public:
    enum {
        CurrentDateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        TypesRole,
        ItemsCountRole
    };

    explicit LocalContentItem(const QDate &date, QObject *parent = 0);

    QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    QDate getDate() const;
    int   getCount() const;

    LocalDayFilterModel *model();
    QString types() const;

    static QHash<int, QByteArray> roleNames();

signals:
    void dataChanged();

public slots:
    void setDate(const QDate &);
    void addActivity(Activity *item);

private:
    QString     m_types;
    int         m_count;
    LocalDayModel  *m_itemModel;
    LocalDayFilterModel  *m_model;
};

#endif // GALLERYITEM_H
