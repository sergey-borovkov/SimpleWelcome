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

#include "activityproxy.h"
#include "activity.h"
#include "localcontentmodel.h"
#include "nepomuksource.h"
#include "previewgenerator.h"

ActivityProxy::ActivityProxy(NepomukSource *source, QObject *parent) :
    QObject(parent),
    m_model(0)
{
    connect(source, SIGNAL(newActivities(QList<Activity *>)), this, SLOT(newData(QList<Activity *>)));
    connect(source, SIGNAL(searchFinished()), SIGNAL(searchFinished()));
    connect(this, SIGNAL(search()), source, SLOT(startSearch()));
}

void ActivityProxy::startSearch()
{
    emit search();
}

void ActivityProxy::newData(QList<Activity *> list)
{
    foreach(Activity * item, list) {
        emit newMonth(item->date().year() , item->date().month(), item->type());  //fill timeScaleModel
    }
    emit newActivities(list);
}

void ActivityProxy::setModel(LocalContentModel *model)
{
    if (m_model)
        m_model = model;
}
