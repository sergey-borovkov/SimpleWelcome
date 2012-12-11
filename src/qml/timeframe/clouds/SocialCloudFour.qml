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
    id: cloudFour

    property date cloudDate
    property variant model
    property int margin: 10

    Item {
        id: cloudRect1
        anchors {
            top: parent.top
            right: parent.right
        }
        width: parent.width * 4 / 7
        height: parent.height * 3 / 5

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
        id: dateLabel
        anchors {
            top : parent.top
            left: parent.left
            right: cloudRect1.left
            rightMargin: margin
        }
        height: parent.height / 10

        Text {
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height

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

    Item {
        id: cloudRect2
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: cloudRect1.height - dateLabel.height
        SocialCloudItem {
            id: rect2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: cloudRect3
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.right
        width: parent.width*2/7
        SocialCloudItem {
            id: rect3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }
    Item {
        id: cloudRect4
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.left
        width: cloudRect3.width
        SocialCloudItem {
            id: rect4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }
}
