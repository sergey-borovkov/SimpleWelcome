/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
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

import QtQuick 1.1
import "../"

Item {
    id: cloudFour
    property date cloudDate
    property variant model
    property int margin: 10

    CloudRect {
        id: cloudRect1
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width*4/7
        height: parent.height*3/5
    }

    Item {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: parent.height/10
        Label{
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
        }
    }

    CloudRect {
        id: cloudRect2
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: cloudRect1.height - dateLabel.height
    }

    CloudRect {
        id: cloudRect3
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.right
        width: parent.width*2/7
    }

    CloudRect {
        id: cloudRect4
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.left
        width: cloudRect3.width
    }
}
