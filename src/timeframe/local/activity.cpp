/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors: Sergey Borovkov <sergey.borovkov@osinit.ru>
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

QString Activity::url() const
{
    return m_url;
}

void Activity::setUrl(const QString &url)
{
    m_url = url;
}

QString Activity::type() const
{
    return m_type;
}

void Activity::setType(const QString &type)
{
    m_type = type;
}

QDate Activity::date() const
{
    return m_date;
}

void Activity::setDate(const QDate &date)
{
    m_date = date;
}

QSize Activity::imageSize() const
{
    return m_imageSize;
}

void Activity::setImageSize(const QSize &imageSize)
{
    m_imageSize = imageSize;
}

QString Activity::mimetype() const
{
    return m_mimetype;
}

void Activity::setMimetype(const QString &mimetype)
{
    m_mimetype = mimetype;
}
