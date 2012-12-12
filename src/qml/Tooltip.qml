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
    id: tooltip
    width: 200
    height: childrenRect.height
    z: 2

    property alias title: titleText.text
    property alias content: contentText.text

    opacity: 0

    BorderImage {
        id: tooltipBackground
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: -3
        }

        border {
            left: 12
            top: 11
            right: 10
            bottom: 11
        }
        width: parent.width + 5 + 5
        height: textsItem.height + 3 + 7

        source: "image://generalicon/asset/tooltip.png"
    }

    Item {
        id: textsItem
        width: parent.width
        height: childrenRect.height + 15 + 15

        Text {
            id: titleText
            anchors {
                top: parent.top
                topMargin: 15
                left: parent.left
                leftMargin: 10
                right: parent.right
                rightMargin: 10
            }
            width: parent.width

            color: "#eee"
            maximumLineCount: 10
            elide: Text.ElideRight
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            font {
                bold: true
                family: "Bitstream Vera Sans"
            }
        }

        Text {
            id: contentText
            anchors {
                top: text.length ? titleText.bottom : titleText.top
                topMargin: text.length ? 10 : 0
                left: parent.left
                leftMargin: 10
                right: parent.right
                rightMargin: 10
            }
            width: parent.width

            color: "#eee"
            maximumLineCount: 20
            elide: Text.ElideRight
            wrapMode: Text.Wrap
            font.family: "Bitstream Vera Sans"
        }
    }

    Behavior on opacity { NumberAnimation { duration: 300 } }
}
