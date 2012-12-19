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

Column {
    id: gridsContainer
    anchors.bottom: parent.bottom
    width: parent.width
    height: childrenRect.height

    signal gridCurrentItemChanged(variant newCurrentItem)

    property variant defaultGroupData
    property variant activeGridGroup
    property variant firstGridGroup
    property variant lastGridGroup
    property variant prevGridGroup: activeGridGroup.prevGridGroup
    property variant nextGridGroup: activeGridGroup.nextGridGroup
    property int gridContainersSpacing: constants.gridWithGroupsSpacing
    property int currentIndex: -1

    spacing: gridContainersSpacing

    function gridsConnectionChanged() {
        var visibleGrids = []
        var i

        for (i = 0; i < children.length; i++)
            if ('gridView' in children[i]) {
                if (children[i].count)
                    visibleGrids.push(i)
                else {
                    children[i].prevGridGroup = children[i]
                    children[i].nextGridGroup = children[i]
                }
            }

        for (i = 0; i < visibleGrids.length; i++) {
            var prevIndex = i - 1 < 0 ? visibleGrids.length - 1 : i - 1
            var nextIndex = i + 1 >= visibleGrids.length ? 0 : i + 1

            children[visibleGrids[i]].prevGridGroup = children[visibleGrids[prevIndex]]
            children[visibleGrids[i]].nextGridGroup = children[visibleGrids[nextIndex]]
        }

        if (visibleGrids.length) {
            activeGridGroup = children[visibleGrids[0]]
            activeGridGroup.gridView.forceMyFocus()
            topBar.forceActiveFocus()
        }
    }

    function addGridGroup(groupData) {
        var prevGridGroup = activeGridGroup
        var groupComponent = Qt.createComponent("GridWithGroup.qml");

        groupData['containerIndex'] = children.length

        var groupObject = groupComponent.createObject(gridsContainer, groupData);
        activeGridGroup = groupObject

        activeGridGroup.gridCurrentItemChanged.connect(gridCurrentItemChanged)
        activeGridGroup.gridMyFocusChanged.connect(gridMyFocusChanged)
        if (prevGridGroup === undefined)
            firstGridGroup = activeGridGroup
        lastGridGroup = activeGridGroup
    }

    function gridMyFocusChanged(index) {
        if (index !== -1)
            currentIndex = index

        for (var i = 0; i < children.length; i++)
            if ('gridView' in children[i]) {
                if (children[i].containerIndex === index) {
                    children[i].myActiveFocus = true
                    activeGridGroup = children[i]
                }
                else
                    children[i].myActiveFocus = false
            }
    }

    function getActiveItem() {
        if (currentIndex != -1)
            for (var i = 0; i < children.length; i++)
                if ('gridView' in children[i] && children[i].containerIndex == currentIndex)
                    return children[i]
        return undefined
    }

    function processKeyboard(key) {
        var activeItem = getActiveItem()
        if (activeItem !== undefined)
            activeItem.gridView.processKeyboard(key)
    }

    Component.onCompleted: {
        if (defaultGroupData !== undefined)
            addGridGroup(defaultGroupData)
    }
}
