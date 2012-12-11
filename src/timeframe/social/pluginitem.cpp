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

#include "pluginitem.h"
#include <socialplugin.h>

#include <QtCore/QVariant>
#include <QtGui/QWidget>

PluginItem::PluginItem(ISocialPlugin *module)
    : m_module(module)
{
    QObject *object = 0;
    if ((object = dynamic_cast<QObject *>(module))) {
        connect(object, SIGNAL(authorized()), SIGNAL(dataChanged()));
        connect(object, SIGNAL(deauthorized()), SIGNAL(dataChanged()));
    }
}

QString PluginItem::id() const
{
    return QString();
}

QVariant PluginItem::data(int role) const
{
    if (role == Name)
        return m_module->name();
    else if (role == Item) {
        QVariant v;
        v.setValue(const_cast<PluginItem *>(this));
        return v;
    } else if (role == Authorized)
        return m_module->authorized();

    return m_module->icon();
}

bool PluginItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    return false;
}

ISocialPlugin *PluginItem::plugin()
{
    return m_module;
}
