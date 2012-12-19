/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Ural Mullabaev <ural.mullabaev@rosalab.ru>
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

GridView {
    id: grid
    anchors {
        left: parent.left
        leftMargin: cellHorizontalSpacing
    }
    width: parent.width
    height: groupName == "Apps" ? gridsListView.height : Math.ceil(count / columns) * groupCellHeight

    signal selectionChangedByKeyboard(variant newCurrentItem)
    signal itemStackingChanged()
    signal myActiveFocusChanged(int containerIndex)

    property variant dataSource
    property variant prevGridGroup
    property variant nextGridGroup
    property int maxCount: -1
    property int indexStartAt: 0
    property bool draggable: false
    property bool enabledSystemDnD: false  // set true to enable system Drag&Drop
    property bool stackable: false // set true to enable icons stacking while Drag&Dropping
    property bool mouseDragChangesGrids: false
    property bool isPopupGroup: false
    property int dndSrcId: gridMouseArea.dndSrcId
    property int dragOutTopMargin: -1
    property int dragOutBottomMargin: -1
    property int columns: constants.gridColumns
    property int cellHorizontalSpacing: Math.max(0, (parent.width - constants.cellWidth * columns) / (columns + 1))
    property bool myActiveFocus: false

    interactive: false
    highlight: highlightComponent
    highlightFollowsCurrentItem: false
    cellWidth: (width - cellHorizontalSpacing) / columns - 1
    cellHeight: groupCellHeight ? groupCellHeight : constants.cellHeight
    delegate: Cell {}

    model: ListModel {
        signal itemClicked(int newIndex)
        signal itemPinnedToggle(int index)
    }

    Keys.onPressed: {
        processKeyboard(event.key)
    }

    function newItemData(itemData) {
        // This is needed for delegate to not blaming unknown variable
        if (itemData.pinned === undefined)
            itemData.pinned = undefined

        if (itemData.stack === undefined)
            itemData.stack = undefined
        model.append(itemData)
    }

    function newItemDataAt(pos, itemData) {
        // This is needed for delegate to not blaming unknown variable
        if (itemData.pinned === undefined)
            itemData.pinned = undefined

        if (itemData.stack === undefined)
            itemData.stack = undefined
        model.insert(pos, itemData)
    }

    function onItemClicked(newIndex) {
        if (newIndex !== -1) {
            var realIndex = model.get(newIndex).id
            if (model.get(newIndex).stack !== undefined) {
                currentIndex = newIndex
                //console.log("onItemClicked::showPopupGroup from: " + newIndex)
                var iconCoords = mapToItem(groupTab, currentItem.x + currentItem.width / 2 - 8, currentItem.y + currentItem.height)
                showPopupGroup(newIndex, model.get(newIndex), iconCoords)
                gridView.myActiveFocus = false
                return
            }
        }
        if (newIndex == -1)
            dataSource.itemClicked(-1)
        else
            dataSource.itemClicked(realIndex, groupName)

        if (groupName === i18n("Recent Applications")) {
            resetContent()
            dataSource.updateContent()
        }
    }

    function onitemPinnedToggle(index) {
        if (dataSource.itemPinnedToggle)
            dataSource.itemPinnedToggle(index)
    }

    function forceMyFocus() {
        myActiveFocus = true
        myActiveFocusChanged(containerIndex)
        //console.log("myActiveFocusChanged to " + containerIndex)
    }

    function resetContent() {
        if (typeof model !== 'undefined')
            model.clear()
    }

    function getCellIndex(inX, inY) {
        var curRow = Math.round((inY - (cellHeight/2)) / cellHeight)
        var curColumn = Math.max(0, Math.min(columns - 1, Math.round((inX - (constants.cellWidth/2)) / cellWidth)))
        var curIndex = Math.max(0, Math.min(count - 1, curRow * columns + curColumn))
        return curIndex
    }

    function stackItemInItem(indexStackingTo, indexDragging, notSaveChanges) {
        var itemDragging = model.get(indexDragging)
        var itemStackingTo = model.get(indexStackingTo)

        var stackArray = itemStackingTo.stack

        if (stackArray === undefined) {
            //console.log("FIRST TIME STACKING")
            stackArray = []
            stackArray.push(root.cloneObject(itemStackingTo))
        }
        else {
            if (stackArray.length >= 4*7) // Limiting stack icons count to 4*7
                return false

            //console.log("STACKING AGAIN")
            for (var i in stackArray) // Checking if item is already present in stack
                if (stackArray[i].id === itemDragging.id) {
                    //console.log("Duplicate ignored")
                    return false
                }
        }
        stackArray.push(root.cloneObject(itemDragging))

        //console.log("----------------- STACKING [" + indexDragging + "] " + model.get(indexDragging).caption + " to [" + indexStackingTo + "] " + model.get(indexStackingTo).caption)


        if (stackArray.length === 2) // First time stacking
            model.setProperty(indexStackingTo, "caption", itemStackingTo.caption + " Group")

        model.setProperty(indexStackingTo, "imagePath", "image://generalicon/stacked/" + itemStackingTo.imagePath.slice(28) + "|" + itemDragging.imagePath.slice(28))
        model.setProperty(indexStackingTo, "stack", stackArray)

        if (notSaveChanges !== true)
            itemStackingChanged()
        return true
    }

    function unstackItemInItem(indexUnstackingFrom, indexDragging) {
        //console.log("----------------- UNSTACKING " + indexDragging + " from " + indexUnstackingFrom)

        var itemDragging = model.get(indexDragging)
        //console.log(gridMouseArea.dndDest + " with " + itemDragging)
        var itemUnstackingFrom = model.get(indexUnstackingFrom)
        //console.log("itemUnstackingFrom: " + itemUnstackingFrom)

        var stackArray = itemUnstackingFrom.stack
        if (stackArray === undefined)
            return false

        var stackIcon = "image://generalicon/stacked/"

        var removeIndex = -1
        for (var i in stackArray) { // If desired item occured - remove it. Otherwise recalculate stack icon
            if (stackArray[i].id === itemDragging.id) {
                //console.log("Removing " +  stackArray[i].id + " - " + stackArray[i].caption + ", its == " + itemDragging.id + " - " + itemDragging.caption)
                removeIndex = i
            }
            else {
                //console.log("Skipping " + stackArray[i].id + " - " + stackArray[i].caption)
                stackIcon += stackArray[i].imagePath.slice(28) + "|"
            }
        }
        if (removeIndex != -1)
            stackArray.splice(removeIndex, 1)
        stackIcon = stackIcon.substring(0, stackIcon.length - 1)

        //console.log("REAL ICON PATH:" + stackIcon + " | items left: " + stackArray.length)

        if (stackArray.length === 1) {
            //console.log("LAST ELEMENT LEFT")
            var innerItem = stackArray[0]
            model.setProperty(indexUnstackingFrom, "stack", undefined)
            model.set(indexUnstackingFrom, innerItem)
        }
        else {
            //console.log("WAS: " + itemUnstackingFrom.imagePath)
            //console.log("TRUNCATING: " + itemDragging.imagePath)
            //console.log("NOEW: " + itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "imagePath", stackIcon)//itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "stack", stackArray)
        }

        // Fixing stacked item ID to match it's first element
        if (removeIndex == 0) {
            //console.log("Fixing stacked item ID: indexUnstackingFrom: " + indexUnstackingFrom + " to " + stackArray[0].id)
            model.setProperty(indexUnstackingFrom, "id", stackArray[0].id)
        }
        itemStackingChanged()
        return true
    }

    function updateItemContent(id, field, data) {
        for (var i = 0; i < model.count; i++)
            if (model.get(i).id === id) {
                model.setProperty(i, field, data)

                // Updating icon cache
                var pathWas = model.get(i).imagePath
                model.setProperty(i, "imagePath", "")
                model.setProperty(i, "imagePath", pathWas)
                break
            }
    }

    function selectOtherGrid(gridWorkingWith, newCurrentIndex) {
        if (gridWorkingWith.highlightItem) {
            gridWorkingWith.highlightItem.moveDuration = 0
            gridWorkingWith.currentIndex = newCurrentIndex
        }
        gridWorkingWith.forceMyFocus()
        if (gridWorkingWith.highlightItem)
            gridWorkingWith.highlightItem.moveDuration = gridWorkingWith.highlightItem.moveDurationConst
        return gridWorkingWith.currentItem
    }

    function processKeyboard(key) {
        var newCurrentItem
        switch (key) {
        case Qt.Key_Left:
            if (currentIndex === 0) { // Need to select other grid when we're at first item
                if ('gridContainersSpacing' in groupRoot.parent && prevGridGroup === lastGridGroup && gridsListView.count > 1) { // Need to switch to other tab
                    if (gridsListView.currentIndex != 0) // There's still some tabs before our current, switching to them
                        gridsListView.decrementCurrentIndex()
                    else // There's no tabs before our current, looping around
                        gridsListView.currentIndex = gridsListView.count - 1

                    gridsListView.currentItem.lastGridGroup.gridView.forceMyFocus()
                    gridsListView.activeGridView.currentIndex = gridsListView.activeGridView.count - 1
                }
                else { // No need to switch to other tab, selecting previous one from available in current container
                    if (prevGridGroup === undefined || prevGridGroup === nextGridGroup && nextGridGroup === groupRoot) { // In case there's only one grid on page or we're in popup grid
                        currentIndex = count - 1
                        break
                    }
                    else if (prevGridGroup) // Selecting previous grid in container
                        newCurrentItem = selectOtherGrid(prevGridGroup.gridView, prevGridGroup.count - 1)
                }
            }

            moveCurrentIndexLeft()
            break
        case Qt.Key_Right:
            if (currentIndex === count - 1) { // Need to select other grid when we're at last item
                if ('gridContainersSpacing' in groupRoot.parent && nextGridGroup === firstGridGroup && gridsListView.count > 1) {
                    if (gridsListView.currentIndex != gridsListView.count - 1)
                        gridsListView.incrementCurrentIndex()
                    else
                        gridsListView.currentIndex = 0

                    gridsListView.currentItem.firstGridGroup.gridView.forceMyFocus()
                    gridsListView.activeGridView.currentIndex = 0
                }
                else {
                    if (nextGridGroup === undefined || prevGridGroup === nextGridGroup && nextGridGroup === groupRoot) { // In case there's only one grid on page or we're in popup grid
                        currentIndex = 0
                        break
                    }
                    else if (nextGridGroup)
                        newCurrentItem = selectOtherGrid(nextGridGroup.gridView, 0)
                }
            }

            moveCurrentIndexRight()
            break
        case Qt.Key_Up:
            if (currentIndex < columns) { // Need to select other grid when we're at first row

                var getPosInPrevGrid = function(prevGrid) {
                    var roundCount = Math.floor((prevGrid.count) / columns) * columns
                    var floor = prevGrid.count - roundCount ? Math.floor((currentIndex % columns) / Math.max(0.1, prevGrid.count - roundCount)) : 1
                    var newCurrent = (currentIndex % columns) + roundCount - columns * Math.min(1, floor)
                    return newCurrent < 0 ? prevGrid.count - 1 : newCurrent
                }

                if ('gridContainersSpacing' in groupRoot.parent && prevGridGroup === lastGridGroup && gridsListView.count > 1) {
                    if (gridsListView.currentIndex != 0)
                        gridsListView.decrementCurrentIndex()
                    else
                        gridsListView.currentIndex = gridsListView.count - 1

                    gridsListView.currentItem.lastGridGroup.gridView.forceMyFocus()
                    gridsListView.activeGridView.currentIndex = getPosInPrevGrid(gridsListView.activeGridView)
                }
                else {
                    if (prevGridGroup === undefined || prevGridGroup === nextGridGroup && nextGridGroup === groupRoot) {
                        currentIndex = getPosInPrevGrid(grid)
                        break
                    }
                    else if (prevGridGroup)
                        newCurrentItem = selectOtherGrid(prevGridGroup.gridView, getPosInPrevGrid(prevGridGroup.gridView))
                }
            }

            moveCurrentIndexUp()
            break
        case Qt.Key_Down:
            if (currentIndex >= count - columns) { // Need to select other grid when we're at last row
                var getPosInNextGrid = function(nextGrid) {
                    var newCurrent = currentIndex % columns
                    return Math.min(nextGrid.count - 1, newCurrent)
                }

                if ('gridContainersSpacing' in groupRoot.parent && nextGridGroup === firstGridGroup && gridsListView.count > 1) {
                    if (gridsListView.currentIndex != gridsListView.count - 1)
                        gridsListView.incrementCurrentIndex()
                    else
                        gridsListView.currentIndex = 0

                    gridsListView.currentItem.firstGridGroup.gridView.forceMyFocus()
                    gridsListView.activeGridView.currentIndex = getPosInNextGrid(gridsListView.activeGridView)
                }
                else {
                    if (nextGridGroup === undefined || prevGridGroup === nextGridGroup && nextGridGroup === groupRoot) { // In case there's only one grid on page or we're in popup grid
                        currentIndex = getPosInNextGrid(grid)
                        break
                    }
                    else if (nextGridGroup) {
                        newCurrentItem = selectOtherGrid(nextGridGroup.gridView, getPosInNextGrid(nextGridGroup.gridView))
                    }
                }
            }

            moveCurrentIndexDown()
            break

        case Qt.Key_Return:
            model.itemClicked(currentIndex)

            break
        case Qt.Key_Enter:
            model.itemClicked(currentIndex)
            break

        case Qt.Key_Backspace:
            //model.itemClicked(-1)
            if (isPopupGroup)
                gridsListView.hideGroup()
            break

        case Qt.Key_Tab:
            console.log("KEY TAB")
            if (isPopupGroup)
                groupLabel.forceActiveFocus()
            break

        case Qt.Key_PageUp:
            gridsListView.decrementCurrentIndex()
            break

        case Qt.Key_PageDown:
            gridsListView.incrementCurrentIndex()
            break

        case Qt.Key_Home:
            gridsListView.currentIndex = 0
            break

        case Qt.Key_End:
            gridsListView.currentIndex = gridsListView.count - 1
            break
        }

        if (key === Qt.Key_Left || key === Qt.Key_Right ||
                key === Qt.Key_Up || key === Qt.Key_Down)
            selectionChangedByKeyboard(newCurrentItem === null ? currentItem : newCurrentItem)
    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }

    Component.onCompleted: {
        //console.log("COMPLETED " + dataSource + " VIEW")
        if (dataSource) {
            dataSource.resetContent.connect(resetContent)
            if (dataSource.updateItemData !== undefined)
                dataSource.updateItemData.connect(updateItemContent)
        }
        model.itemClicked.connect(onItemClicked)
        model.itemPinnedToggle.connect(onitemPinnedToggle)

        if ('group' in model)
            model.group = groupName
    }
}

