/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

import QtQuick 1.0

Item {
    property alias color : rectangle.color
    property alias radius : rectangle.radius

    Rectangle {
        id: rectangle;
        anchors.fill: parent;
        radius: 3;
        color: "#172634"
        border {
            color: "#687584"
            width: 1
        }

        BorderImage {
            z: -1
            anchors {
                bottomMargin: -8
                fill: parent
            }
            width: rectangle.width
            height: rectangle.height
            border {
                left: 10
                top: 10
                right: 10
                bottom: 10
            }
            source: "images/shadow.png"; smooth: true
        }
    }
    states: [
        State {
            name: "notEnabled"
            PropertyChanges {
                target: rectangle
                color: "#2A3947"
                border.color: "#4B5E6E"
            }
        }
    ]
}
