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
    id: popupFrame
    width: parent.width
    z: 1

    property int slideHeight: 0
    property int stackedIconIndex: -1
    property alias gridGroup: appsGrid
    property alias arrowX: arrow.x
    property alias groupTitle: appsGrid.groupName

    clip: true
    state: "CLOSED"

    function groupCloseCompleted() {
        if (gridMouseArea.dndSrcId === -1)
            gridsListView.dndStateChanged(false)
    }

    Item {
        id: popupGroupWrapper
        width: parent.width
        height: childrenRect.height + 3

        Image {
            id: arrow
            anchors {
                bottom: groupBorder.top
                bottomMargin: -1
            }
            z: 1

            source: "image://generalicon/asset/popup_arrow.png"
        }

        Rectangle {
            id: groupBorder
            anchors {
                top: parent.top
                topMargin: 14
            }
            x: -1
            width: parent.width + 1
            height: childrenRect.height + 64 - 10 + (popupFrame.y + childrenRect.height + 64 - 10 + 3 + 20 < tabRoot.height ? 20 : 0)

            color: Qt.rgba(0.078, 0.125, 0.172, 0.97)
            focus: true
            border.color: Qt.rgba(1, 1, 1, 0.31)

            GridWithGroup {
                id: appsGrid
                anchors {
                    top: parent.top
                    topMargin: 32
                }
                width: parent.width
                height: childrenRect.height

                isPopupGroup: true
                draggable: true
                dragOutTopMargin: 72
                dragOutBottomMargin: 33
                mouseDragChangesGrids: true
                groupCellHeight: (tabRoot.height - 75 - tabRoot.height / 5 - 10) / 4 - 1
                dataSource: dataSource_Apps
            }
        }

        Image {
            source: "image://generalicon/asset/popup_group_shadow.png"
            anchors.top: groupBorder.bottom
            anchors.topMargin: 1
            width: groupBorder.width
            z: 1
        }
    }

    states: [
        State {
            name: "CLOSED"
            PropertyChanges {
                target: popupFrame
                height: 0
                slideHeight: 0
            }
        },

        State {
            name: "OPEN"
            PropertyChanges {
                target: popupFrame
                height: childrenRect.height
                slideHeight: y + height < tabRoot.height ? 0 : -(tabRoot.height - (y + height - 6))
            }
        }
    ]

    transitions: [
        Transition {
            from: "CLOSED"
            to: "OPEN"

            NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "OPEN"
            to: "CLOSED"

            SequentialAnimation {
                NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
                ScriptAction { script: popupFrame.groupCloseCompleted() }
            }
        }
    ]

    Component.onCompleted: {
        gridGroup.groupNameChanged.connect(gridsListView.groupNameChanged)
    }
}
