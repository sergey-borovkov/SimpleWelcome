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

Item {
    id: sessionButton

    property alias imgNormal: normalButton.source
    property alias imgHover: hoverButton.source
    property alias imgPressed: pressedButton.source

    function onButtonClicked() { }

    Image {
        id: normalButton
        anchors.centerIn: parent
        sourceSize {
            width: parent.width
            height: parent.height
        }
    }

    Image {
        id: hoverButton
        anchors.centerIn: parent
        sourceSize {
            width: parent.width
            height: parent.height
        }

        opacity: 0

        Behavior on opacity {
            NumberAnimation { duration: 100; }
        }
    }

    Image {
        id: pressedButton
        anchors.centerIn: parent
        sourceSize {
            width: parent.width
            height: parent.height
        }

        opacity: 0
    }

    MouseArea {
        id: sessionButtonMouseArea
        anchors.fill: parent

        hoverEnabled: true

        onClicked: onButtonClicked()
    }

    states: [
        State {
            name: "normal";
            when: !sessionButtonMouseArea.containsMouse
            PropertyChanges { target: hoverButton; opacity: 0; }
        },

        State {
            name: "hover";
            when: sessionButtonMouseArea.containsMouse && !sessionButtonMouseArea.pressed
            PropertyChanges { target: hoverButton; opacity: 1 }
        },

        State {
            name: "pressed"
            when: sessionButtonMouseArea.pressed
            PropertyChanges { target: pressedButton; opacity: 1 }
        }
    ]
}
