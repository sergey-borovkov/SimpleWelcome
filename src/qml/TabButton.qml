/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
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
    id: button
    width: constants.iconSize >= 64 ? 200 : 180
    height: 27

    signal buttonClick()

    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"
    property bool active: false
    property bool pressable: false
    property bool pressedAndHovered: buttonMouseArea.pressed && buttonMouseArea.containsMouse

    property alias label: buttonLabel.text

    Text {
        id: buttonLabel
        anchors {
            centerIn: parent
            horizontalCenterOffset: pressable && pressedAndHovered ? 1 : 0
            verticalCenterOffset: pressable && pressedAndHovered ? 1 : 0
        }

        color: active ? "black" : "white"

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent

        hoverEnabled: true

        onClicked: buttonClick()
    }
}
