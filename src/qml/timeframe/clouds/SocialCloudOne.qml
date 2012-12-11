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
    id: cloudOne
    property date cloudDate
    property variant model

    Item {
        id: cloudRect1
        anchors.centerIn: parent
        width: parent.width*3/4
        height: parent.height*3/4
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }

    }

    Text{
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
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
