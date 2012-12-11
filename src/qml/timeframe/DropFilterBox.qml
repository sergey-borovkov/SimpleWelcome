/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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

import QtQuick 1.1

Item {
    id: dropFilterBox
    width: 330
    height: 30

    property alias model: filterListView.model
    property alias text: currentItemText.text
    property alias view: filterListView
    property string name: ""
    property string selectedText: ""

    signal currentIndexChanged

    //set droplistbox colapse by clicking out of his area
    MouseArea {
        id: modal
        anchors.fill: parent
        enabled: true
        onClicked: {
            dropFilterBox.state = "current"
        }
    }

    Item {
        id: currentItem
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: 30

        ItemRectangle {
            id: background
            anchors.fill: parent
            anchors.bottomMargin: 2
            radius: 4
            state: "notEnabled"
        }

        MouseArea{
            id: currentMouseArea
            anchors.fill: parent
            onClicked: {
                if (dropFilterBox.state === "")
                    dropFilterBox.state = "current"
                else if  (dropFilterBox.state === "current")
                    dropFilterBox.state = "open"
                else
                    dropFilterBox.state = "current"
            }
        }
        Text {
            id: currentItemText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: dropDownImage.left
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
            color: "white"
            text: name
        }
        Image {
            id: dropDownImage
            source: "images/drop-down.png"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            smooth: true
            fillMode: Image.PreserveAspectFit
            width: 24
        }
    }
    Item {
        id: listBox
        width: parent.width
        anchors.top: currentItem.bottom
        anchors.bottom: parent.bottom

        ItemRectangle {
            id: listBackground
            anchors.fill: parent
            anchors.topMargin: 3
            visible: false
            radius: 4
        }

        ListView {
            id: filterListView
            anchors.fill: parent
            anchors.topMargin: 3
            interactive: false
            clip: true
            delegate: Item {
                id: filterDelegate
                width: parent.width
                height: 30
                property alias text : delegateText.text
                Text {
                    id: delegateText
                    anchors.fill: parent
                    //horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    color: "white"
                    text: getMenuItemText(itemText)
                    font.bold: filterDelegate.ListView.isCurrentItem ? true : false
                }
                MouseArea {
                    id: delegateMouseArea
                    anchors.fill: parent
                    onClicked: {
                        dropFilterBox.state = "current"
                        filterListView.currentIndex = index
                        selectedText = filterListView.currentItem.text
                        currentIndexChanged()
                    }
                }
            }
            onCurrentIndexChanged: {
                if (currentIndex === -1)
                    return
                if (currentIndex === 0)
                    currentItemText.text = name
                else {
                    currentItemText.text = filterListView.currentItem.text
                }

            }
        }
    }
    states: [
        State {
            name: "current"
            PropertyChanges { target: background; state: "" }
        },
        State {
            name: "open" ; extend: "current"
            PropertyChanges { target: dropFilterBox; height: 30 + model.count*30 }
            ParentChange{target: modal; parent: timeFrameTab}
            PropertyChanges { target: modal; enabled: true }
            PropertyChanges { target: listBackground; visible: true }
        }
    ]
    transitions: [
        Transition {
            from: "current"
            to: "open"
            NumberAnimation { properties: "height"; duration: 100 }
        },
        Transition {
            from: "open"
            to: "current"
            NumberAnimation { properties: "height"; duration: 100 }
        }
    ]
}

