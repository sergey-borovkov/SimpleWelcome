/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
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
import ".."

Item {
    id: cloudTwo

    property date cloudDate
    property variant model

    Item {
        id: cloudRect1
        anchors {
            bottom: parent.bottom
            left: parent.left
        }
        width: parent.width * 1 / 3
        height: parent.height / 2

        SocialCloudItem {
            id: rect1
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter:  parent.verticalCenter
            }
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: cloudRect2
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        width: parent.width * 2 / 3 - 20
        height: parent.height * 4 / 5

        SocialCloudItem {
            id: rect2
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter:  parent.verticalCenter
            }
            width: parent.width
            height: parent.height
        }
    }

    Item {
        anchors {
            top : parent.top
            left: parent.left
            right: cloudRect2.left
            bottom: cloudRect1.top
            rightMargin: 10
        }

        Text {
            id: dateLabel
            anchors {
                bottom: parent.bottom
                bottomMargin: 10
                horizontalCenter: parent.horizontalCenter
            }
            height: labelHeight

            text: Qt.formatDate(cloudDate , "dd MMM yyyy")
            horizontalAlignment: Text.AlignHCenter

            style: Text.Raised
            styleColor: "#000"
            color:  "white"
            font {
                family: "Bitstream Vera Sans"
                pointSize: constants.iconTextSize
                bold: true
            }
        }
    }
}
