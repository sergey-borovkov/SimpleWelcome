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

Image{
    id: mainImage

    Image {
        id: waitIndicator
        anchors.centerIn: parent

        source: "images/indicator-shadow.png"
        visible: false

        Image {
            id: can

            source: "images/can2.png"
            smooth: true
        }

        Behavior on opacity { NumberAnimation{ duration: 1000 }}
    }

    Timer {
        id: waitTimer

        interval: 10
        running: false
        repeat: true

        onTriggered: can.rotation = can.rotation +10
    }

    Column {
        id: errorItem
        anchors.centerIn: parent

        visible: false

        Image {
            id: errorIcon
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/error-icon.png"
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            color: "white"
            text: i18n("Image is currently not available")
        }
    }

    states: [
        State {
            name: "loading"; when: mainImage.status === Image.Loading
            PropertyChanges { target: waitIndicator; visible: true }
            PropertyChanges { target: waitTimer; running: true }
        },
        State {
            name: "error"; when: mainImage.status === Image.Error
            PropertyChanges { target: errorItem; visible: true }
        }
    ]
}
