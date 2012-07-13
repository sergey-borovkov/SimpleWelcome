/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
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

Rectangle {
    id: button

    property string  imageUrl;
    property color onHoverColor: Qt.darker( borderColor, 1.3 )
    property color borderColor: "#7A8182"
    property color buttonColor: "#61BDCACD"

    //set appearance properties
    smooth: true
    width: 100
    height: 100
    clip: true
    color: "#00000000"

    Image {
        id: buttonImage
        source: imageUrl
        anchors.centerIn: parent
        smooth: true
        fillMode: Image.PreserveAspectFit
        opacity: 0

        Behavior on opacity { NumberAnimation{ duration: 300} }
    }

    //buttonClick() is callable and a signal handler, onButtonClick is automatically created
    signal buttonClick()

    //define the clickable area to be the whole rectangle
    MouseArea {
        id: buttonMouseArea
        smooth: true
        anchors.fill: parent    //stretch the area to the parent's dimension
        onClicked: buttonClick()
        hoverEnabled: true

        onEntered: {
            buttonImage.opacity = 1
        }

        onExited: {
            buttonImage.opacity = 0
        }
    }

    //scale the button when pressed
    scale: buttonMouseArea.pressed ? 1.05 : 1.00
    Behavior on scale { NumberAnimation { duration: 50 } }
}
