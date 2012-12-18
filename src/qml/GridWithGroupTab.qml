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
import Private 0.1

Item {
    id: tabRoot
    anchors.fill: parent

    // Dict-defined
    property variant groups
    property bool isForceOnOneScreen: false
    // Other
    property bool isPopupOpened: popupFrame.state === "OPEN"
    property int stackCellOpenedId: -1
    property variant dataSources
    property variant iconPositions

    property alias tabListView: gridsListView
    property alias gridMouseArea: gridsListView.gridMouseArea

    clip: true

    function updateGridsContent() {
        gridsListView.updateGridsContent()
    }

    function processKeyboard(key) {
        if (popupFrame.state === "OPEN")
            popupFrame.gridGroup.gridView.processKeyboard(key)
        else if (gridsListView.currentItem)
            gridsListView.currentItem.processKeyboard(key)
    }

    function closePopup() {
        gridsListView.hideGroup()
    }

    Item {
        id: tabWrapper
        anchors {
            top: parent.top
            topMargin: -popupFrame.slideHeight
        }
        width: parent.width
        height: parent.height + popupFrame.slideHeight

        function draggedOut(item) {
            gridsListView.hideGroup(false)
            //console.log(item.caption + " GOT")

            var gridView = gridsListView.activeGridView
            gridView.newItemData(item)
            gridView.unstackItemInItem(popupFrame.stackedIconIndex, gridView.count - 1)

            gridMouseArea.skipMoveAnimation = true
            // If new page needed
            if (gridView.maxCount !== -1 && gridView.count > gridView.maxCount) {
                gridView.model.move(gridView.count - 2, gridView.count - 1, 1)

                //console.log("Drag OUT and push pop is here")
                gridsListView.gridIconPushPop("Apps", true)
            }
            gridMouseArea.updateCurrentGrid()

            gridMouseArea.startDragging(gridsListView.activeGridView.count - 1)
            popupFrame.stackedIconIndex = -1
        }

        function showTooltip(itemData, item) {
            tooltip.title = itemData.caption
            if (itemData.stack === undefined)
                tooltip.content = itemData.description
            else
                tooltip.content = ""

            tooltip.x = mapFromItem(item, item.width/2, 0).x - tooltip.width / 2
            tooltip.y = mapFromItem(item, 0, item.height).y + 4 + 3
            if (tooltip.y + tooltip.height > tabWrapper.height)
                tooltip.y = mapFromItem(item, 0, 0).y - tooltip.height - 5 + 6
            if (tooltip.y < 0)
                tooltip.y = 0
            tooltip.opacity = 1
        }

        function hideTooltip() {
            tooltip.opacity = 0
        }

        GridWithGroupTabListView {
            id: gridsListView
        }

        // Scroll using mouse wheel
        WheelArea {
            id: wheelArea
            anchors.fill: parent

            onScrollVert: {
                if (popupFrame.state === "CLOSED") {
                    if (gridMouseArea.pressed && gridMouseArea.dndSrcId != -1) {
                        gridMouseArea.dragIconToPrevNextTab(delta <= 0)
                    }
                    else {
                        if (delta > 0)
                            gridsListView.decrementCurrentIndex()
                        else
                            gridsListView.incrementCurrentIndex()
                    }
                }
            }
        }

        // Bottom dots for displaying current tab index
        Row {
            id: bottomDots
            anchors {
                top: gridsListView.bottom;
                horizontalCenter: gridsListView.horizontalCenter
            }
            width: childrenRect.width
            height: 30

            spacing: 20
            visible: gridsListView.count > 1

            Repeater {
                model: gridsListView.count

                Rectangle {
                    id: dot
                    width: 6
                    height: 6

                    radius: 3
                    color: gridsListView.currentIndex == index ? "white" : "gray"

                    MouseArea {
                        id: mouse
                        width: 26
                        height: 30
                        anchors.centerIn: parent
                        onClicked: gridsListView.currentIndex = index
                    }

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
            }
        }

        // Background darkener for displaying popup grid with stacked icons
        Rectangle {
            anchors {
                top: parent.top
                topMargin: -15
            }
            width: parent.width
            height: parent.height + 28

            color: Qt.rgba(0, 0, 0, 0.6)
            opacity: popupFrame.state == "OPEN"

            Behavior on opacity {
                NumberAnimation { duration: 200; }
            }
        }

        PopupFrame {
            id: popupFrame
        }

        Tooltip {
            id: tooltip
        }

    } // tabWrapper
} // tabRoot
