/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

ListView {
    id: socialView
    spacing: 10
    orientation: ListView.Horizontal

    delegate: ShadowRectangle {
        width: 300
        height: 300

        Text {
            id: msg
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: 200
            text: message
        }
        Image {
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            width: Math.min( sourceSize.width, parent.width)
            height: Math.min( sourceSize.height, parent.height)
            smooth: true
            source: picture
        }
    }
}
