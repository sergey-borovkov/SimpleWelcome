/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
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

import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width

    property variant grid: groupTab.tabListView.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab

        property int tabCellHeight: (tabListView.height - (constants.textToGridSpacing + constants.groupTextHeight)*3 - constants.gridWithGroupsSpacing*2) / 4

        isForceOnOneScreen: true
        groups: [
            {
                groupName: i18n("Recent Applications"),
                dataSource: dataSource_RecentApps,
                draggable: true,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight
            },
            {
                groupName: i18n("Places"),
                dataSource: dataSource_Favorites,
                //draggable: true,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight
            },
            {
                groupName: i18n("Recent Documents"),
                dataSource: dataSource_Documents,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight,
                isClearAvailable: true
            },
        ]
    }
}
