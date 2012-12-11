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
Item{
    id: cloudThree
    property date cloudDate
    property variant model
    property int margin: 10


    Item {
        id: cloudRect1
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height*3/5
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 - margin
        height: parent.height*4/5
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
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.left
        anchors.rightMargin: margin
        width: parent.width*1/4
        SocialCloudItem {
            id: rect3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        anchors.top : cloudRect2.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: cloudRect3.right
        anchors.leftMargin: margin

        Text{
            id: dateLabel
            anchors.centerIn: parent
            width: 100
            height: labelHeight
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            horizontalAlignment: Text.AlignHCenter

            style: Text.Raised
            styleColor: "#000"
            color:  "white"
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize
            font.bold: true
        }
   }
}
