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

#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

#include <listitem.h>

class ISocialPlugin;

class PluginItem : public QObject, public ListItem
{
    Q_OBJECT
public:
    enum {
        Name,
        Icon,
        Item,
        Authorized
    };

    PluginItem(ISocialPlugin *module);
    virtual ~PluginItem() {}
    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    ISocialPlugin *plugin();
    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(Name, "name");
        roles.insert(Icon, "icon");
        roles.insert(Item, "item");
        roles.insert(Authorized, "authorized");
        return roles;
    }

signals:
    void dataChanged();

private:
    ISocialPlugin *m_module;
};

Q_DECLARE_METATYPE(PluginItem *)

#endif // PLUGINITEM_H
