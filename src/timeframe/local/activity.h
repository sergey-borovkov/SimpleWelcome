/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QMetaType>

class Activity
{
public:
    explicit Activity(const QString &url, const QString &type, const QDate &date, const QString &mimetype, const QSize &imageSize)
        : m_mimetype(mimetype), m_url(url), m_type(type), m_date(date), m_imageSize(imageSize) {}

    QString mimetype() const;
    void setMimetype(const QString &mimetype);

    QString url() const;
    void setUrl(const QString &url);

    QString type() const;
    void setType(const QString &type);

    QDate date() const;
    void setDate(const QDate &date);

    QSize imageSize() const;
    void setImageSize(const QSize &imageSize);

private:
    QString m_mimetype;
    QString m_url;
    QString m_type;
    QDate m_date;
    QSize m_imageSize;
};

Q_DECLARE_METATYPE(Activity *)

#endif // ACTIVITY
