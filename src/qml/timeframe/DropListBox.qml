/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Julia Mineeva <julia.mineeva@osinit.ru>
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
    id: dropListBox

    property string name;
    property string selectedText: dropboxListSelection.model.get( 0 ).itemText
    property alias model: dropboxListSelection.model
    property alias selectedIndex: dropboxListSelection.currentIndex

    signal comboClicked
    signal selectedIndexChanged
    signal clicked

    width: dropboxButtonText.paintedWidth + 24 + dropboxDrop.width
    height: 30
    focus: true
    state: ""

    Button {
        id: dropboxButtonBody
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        z: 100

        ButtonText {
            id: dropboxButtonText
            text: name
            anchors.left: parent.left
        }

        MouseArea {
            id: dropboxButtonMouseArea
            anchors.fill: parent

            onPressed: {
                dropboxListBody.hide()
                dropListBox.clicked()
            }
        }

        Button {
            id: dropboxDrop
            anchors.right: parent.right
            height: parent.height
            width: parent.height

            Image {
                anchors.centerIn: parent

                source: "images/drop-down.png"
                smooth: true
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    dropboxListBody.toggle()
                    dropListBox.clicked()
                }
            }
        }
    }

    Button {
        id: dropboxListBody
        anchors {
            top: dropboxButtonBody.bottom
            left: dropboxButtonBody.left
        }
        width: dropboxButtonBody.width
        height: 0
        z: 200

        property int unfoldedHeight: dropboxButtonBody.height * dropboxListSelection.count

        enabled: false
        clip: true
        focus: true

        function toggle()
        {
            if ( height == 0 )
                height = unfoldedHeight
            enabled = true
        }

        function hide()
        {
            enabled = false
            height = 0
        }

        function show()
        {
            height = unfoldedHeight
        }

        ListView {
            id: dropboxListSelection
            anchors.fill: parent

            currentIndex: 0
            focus: true

            delegate: Item {
                width: dropboxButtonBody.width
                height: dropboxButtonBody.height
                z: 1000

                ButtonText {
                    anchors.fill: parent

                    text: getMenuItemText(itemText)
                    font {
                        bold: itemText == selectedText
                        pointSize: 12
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        var prevSelection = selectedText
                        if (dropboxListSelection.model.get(index).itemText != prevSelection) {
                            dropboxListSelection.currentIndex = index;
                            selectedText = dropboxListSelection.model.get(index).itemText
                            dropboxListSelection.focus = true
                            dropboxListBody.hide()
                            selectedIndexChanged()
                        }
                        comboClicked()
                    }
                }
            }
        }
    }

    MouseArea {
        id: componentMouseArea
        anchors {
            right: parent.right
            left: parent.left
            top: parent.top
        }
        height: (dropboxListBody.enabled * dropboxListBody.height) + parent.height

        hoverEnabled: true
        onExited: dropboxListBody.hide()
    }

    states: State {
        name: "current"
        PropertyChanges { target: dropboxButtonBody; state: "selected" }
        PropertyChanges { target: dropboxDrop; state: "selected" }
        PropertyChanges { target: dropboxListBody; state: "selected" }
    }
}
