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

#ifndef GALLERYLISTER_H
#define GALLERYLISTER_H

#include "nepomuksource.h"

#include <QtCore/QObject>
#include <QtCore/QDate>

class Activity;
class LocalContentModel;

class ActivityProxy : public QObject
{
    Q_OBJECT
public:
    explicit ActivityProxy(NepomukSource *source, QObject *parent = 0);
    void setModel(LocalContentModel *model);

signals:
    void newActivities(QList<Activity *>);
    void newMonth(int, int , QString type);
    void searchFinished();
    void search();

public slots:
    void startSearch();

private slots:
    void newData(QList<Activity *>);

private:
    LocalContentModel *m_model;
};

#endif // GALLERYLISTER_H
