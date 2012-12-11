/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#ifndef PLUGINMODEl_H
#define PLUGINMODEl_H

#include <listmodel.h>

class PluginModel : public ListModel
{
    Q_OBJECT
public:
    PluginModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    virtual void appendRows(const QList<ListItem *> &items);

    Q_INVOKABLE bool isAuthorized(int i) const;

private slots:
    void itemChanged();
};

#endif // PLUGINMODEl_H
