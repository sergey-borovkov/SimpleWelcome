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

#include "pluginitem.h"
#include "pluginmodel.h"
#include <socialplugin.h>

PluginModel::PluginModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

void PluginModel::appendRows(const QList<ListItem *> &items)
{
    foreach(ListItem * item, items) {
        PluginItem *plugin = dynamic_cast<PluginItem *>(item);
        connect(plugin, SIGNAL(dataChanged()), SLOT(itemChanged()));
    }

    ListModel::appendRows(items);
}

bool PluginModel::isAuthorized(int i) const
{
    QModelIndex idx = ListModel::index(i);
    return ListModel::data(idx, PluginItem::Authorized).toBool();
}

void PluginModel::itemChanged()
{
    PluginItem *item = static_cast<PluginItem *>(sender());
    QModelIndex index = indexFromItem(item);
    emit dataChanged(index, index);
}
