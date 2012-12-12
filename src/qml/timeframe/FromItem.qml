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
    height: fromImage.height

    property alias userImage: fromImage.source
    property alias userName: fromName.text

    Image {
        id: fromImage
        width: 24
        height: 24
        anchors {
            left: parent.left
            leftMargin: 5
            rightMargin: 5
        }

        source: "images/user.png"
        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: fromName
        anchors {
            left: fromImage.right
            right: parent.right
            leftMargin: 5
            rightMargin: 5
        }

        color: "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
