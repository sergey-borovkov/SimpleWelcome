/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 *
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

#include "appentity.h"

#include <KDebug>

AppEntity::AppEntity() : QDeclarativeItem(), m_valid(false)
{

}

AppEntity::AppEntity(KService *service) : QDeclarativeItem(), m_valid(true)
{
    m_name = service->name();
    m_isApp = true;
    m_display = true;
    m_iconName = service->icon();
    m_comment = service->comment();

    m_genericName = service->genericName();
    m_menuId = service->menuId();
    m_entryPath = service->entryPath();

    /*
    kDebug() << "----------------";
    kDebug() << "Actions: " << service->actions().size();
    kDebug() << "desktopEntryName: " << service->desktopEntryName();
    kDebug() << "desktopEntryPath: " << service->desktopEntryPath();
    kDebug() << "entryPath: " << service->entryPath();
    kDebug() << "exec: " << service->exec();
    kDebug() << "path: " << service->path();
    kDebug() << "type: " << service->type();
    */
}

AppEntity::AppEntity(KServiceGroup *group) : QDeclarativeItem(), m_valid(true)
{
    m_name = group->name();
    m_isApp = false;
    m_display = true;
    m_iconName = group->icon();
    m_comment = group->comment();

    KServiceGroup::List list = group->entries(true, // sorted
                                              true, // exclude no display
                                              false, // exclude separator items
                                              false // don't sort by generic name
                                              );

    for(KServiceGroup::List::ConstIterator it = list.begin(); it != list.end(); it++)
    {
        const KSycocaEntry *sycocaEntry = ((*it).data());

        m_entries.append(sycocaEntry->name());
    }
}

AppEntity::~AppEntity(void)
{

}

QDebug operator<<(QDebug dbg, const AppEntity *entity)
{
    if(entity->isApp())
        dbg.nospace() << "AppEntity(app): " << entity->name() << ", " << entity->genericName() << ")";
    else
        dbg.nospace() << "AppEntity(group): " << entity->name() << ", " << entity->entries().size() << ")";

    return dbg.space();
}

#include "appentity.moc"
