/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeca@osinit.ru>
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

Rectangle {
    id: button
    width: 100
    height: 100

    property string  imageUrl;
    property color onHoverColor: Qt.darker( borderColor, 1.3 )
    property color borderColor: "#7A8182"
    property color buttonColor: "#61BDCACD"

    //set appearance properties
    smooth: true
    clip: true
    color: "#00000000"

    Image {
        id: buttonImage
        anchors.centerIn: parent

        source: imageUrl
        smooth: true
        fillMode: Image.PreserveAspectFit
        opacity: 0

        Behavior on opacity { NumberAnimation{ duration: 300} }
    }

    signal buttonClick()

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent

        smooth: true
        onClicked: buttonClick()
        hoverEnabled: true

        onEntered: {
            buttonImage.opacity = 1
        }

        onExited: {
            buttonImage.opacity = 0
        }
    }

    scale: buttonMouseArea.pressed ? 1.05 : 1.00
    Behavior on scale { NumberAnimation { duration: 50 } }
}
