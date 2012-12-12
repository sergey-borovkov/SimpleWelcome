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
    id: groupRoot

    width: parent.width
    height: childrenRect.height

    signal gridCurrentItemChanged(variant newCurrentItem)
    signal showPopupGroup(int index, variant stackItemData, variant iconCoords)
    signal gridMyFocusChanged(int containerIndex)
    signal groupNameChanged(string newName)

    // constants
    property int groupCellHeight: constants.cellHeight
    property int textToGridSpacing: constants.textToGridSpacing
    property int textHeight: constants.groupTextHeight
    property int containerIndex: 0

    // Dict-defined properties
    property alias groupName: groupLabel.text
    property alias dataSource: iconGridView.dataSource
    property alias draggable: iconGridView.draggable
    property alias enabledSystemDnD: iconGridView.enabledSystemDnD
    property alias groupNameVisible: groupLabelWrapper.visible
    property alias stackable: iconGridView.stackable
    property alias mouseDragChangesGrids: iconGridView.mouseDragChangesGrids
    property bool  isClearAvailable: false
    // iconGridView aliases
    property alias prevGridGroup: iconGridView.prevGridGroup
    property alias nextGridGroup: iconGridView.nextGridGroup
    property alias maxCount: iconGridView.maxCount
    property alias indexStartAt: iconGridView.indexStartAt
    property alias count: iconGridView.count
    property alias gridView: iconGridView
    property alias dragOutTopMargin: iconGridView.dragOutTopMargin
    property alias dragOutBottomMargin: iconGridView.dragOutBottomMargin
    property alias myActiveFocus: iconGridView.myActiveFocus
    property alias isPopupGroup: iconGridView.isPopupGroup

    property alias broomIcon: broomIcon

    spacing: textToGridSpacing
    //visible: iconGridView.count != 0

    Item {
        id: groupLabelWrapper
        anchors.left: parent.left
        width: parent.width
        height: groupLabel.text || isPopupGroup ? textHeight : 0

        TextInput {
            id: groupLabel
            anchors {
                left: parent.left
                leftMargin: 39
            }

            readOnly: !isPopupGroup
            activeFocusOnPress: isPopupGroup
            font {
                family: isPopupGroup ? "Arial, Helvetica, Nimbus Sans L, Liberation Sans, FreeSans, Sans-serif" : "Bitstream Vera Sans"
                weight: isPopupGroup ? Font.DemiBold : Font.Bold
                pixelSize: isPopupGroup ? 23 : 14
            }
            color: "#eee"

            onTextChanged: {
                if (isPopupGroup)
                    groupNameChanged(groupName)
            }

            onActiveFocusChanged: {
                if (activeFocus)
                    iconGridView.myActiveFocus = false
            }

            Keys.onEnterPressed: {
                if (isPopupGroup)
                    gridsListView.hideGroup()
            }

            Keys.onReturnPressed: {
                if (isPopupGroup)
                    gridsListView.hideGroup()
            }
            Keys.onTabPressed: {
                iconGridView.myActiveFocus = true
                event.accepted = false
            }

            /*BorderImage {
                id: textBackground
                anchors {
                    left: parent.left
                    leftMargin: -10
                    top: parent.top
                    topMargin: -7
                }
                border {
                    left: 6
                    right: 6
                    top: 6
                    bottom: 6
                }
                width: groupLabel.width + 20
                height: 30
                z: -1

                visible: !groupLabel.readOnly
                source: "image://generalicon/asset/search_bar_bg.png"
            }*/
        }

        Image {
            id: pen
            anchors {
                left: groupLabel.right
                leftMargin: 15
                verticalCenter: groupLabel.verticalCenter
                verticalCenterOffset: 2
            }

            visible: isPopupGroup
            source: "image://generalicon/asset/pen.png"
        }

        Image {
            id: lineDecoration
            anchors {
                left: parent.left
                leftMargin: 3
                bottom: parent.bottom
                bottomMargin: -2
            }
            width: parent.width - 3 - 42 - 22
            height: 3
            z: 1

            visible: !isPopupGroup
            source: "image://generalicon/asset/group_line.png"
        }

        Image {
            id: broomIcon
            anchors {
                //right: parent.right
                //rightMargin: 39
                left: groupLabel.right
                leftMargin: 10
                verticalCenter: groupLabel.verticalCenter
            }

            opacity: isClearAvailable && myActiveFocus && gridMouseArea.grid === iconGridView
            source: "image://generalicon/asset/clear.png"

            function click() {
                if (dataSource.clearClicked !== undefined)
                    dataSource.clearClicked()
            }

            Behavior on opacity {
                NumberAnimation { duration: 300 }
            }

            states: [
                State {
                    name: "broomHovered"
                    when: gridMouseArea.broomHovered && !gridMouseArea.pressed
                    PropertyChanges {target: broomIcon; source: "image://generalicon/asset/clear_hover.png"}
                },

                State {
                    name: "broomPressed"
                    when: gridMouseArea.broomHovered && gridMouseArea.pressed
                    PropertyChanges {target: broomIcon; source: "image://generalicon/asset/clear_pressed.png"}
                }
            ]
        }
    }

    IconGridView {
        id: iconGridView
    }

    Component {
        id: highlightComponent

        Item {
            id: gridSelection
            x: iconGridView.currentItem ? iconGridView.currentItem.x : 0
            y: iconGridView.currentItem ? iconGridView.currentItem.y : 0
            width: iconGridView.currentItem ? iconGridView.currentItem.width : 0
            height: iconGridView.currentItem ? iconGridView.currentItem.height : 0

            property int animationDuration: 150
            property int moveDurationConst: 150
            property int moveDuration: moveDurationConst

            opacity: myActiveFocus ? 1 : 0

            BorderImage {
                id: gridSelectionPic
                anchors {
                    fill: parent
                    rightMargin: -2
                    bottomMargin: -2
                }
                border {
                    left: 5
                    right: 7
                    top: 5
                    bottom: 7
                }

                source: "image://generalicon/asset/grid_selection.png"
            }

            Behavior on x {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on y {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on width {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on height {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on opacity {
                NumberAnimation { duration: animationDuration }
            }
        }
    }

    states: [
        State {
            name: "clipped"
            PropertyChanges {
                target: groupRoot
                clip: true
            }
        },

        State {
            name: "unclipped"
            PropertyChanges {
                target: groupRoot
                clip: false
            }
        }
    ]

    transitions: Transition {
        from: "clipped"
        to: "unclipped"
        PropertyAnimation { target: groupRoot; properties: "clip"; duration: 300 }
    }

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
        iconGridView.myActiveFocusChanged.connect(gridMyFocusChanged)
    }
} // groupRoot
