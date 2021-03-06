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
    id: cloudFive

    property date cloudDate
    property variant model
    property int margin: 10

    Item {
        id: dateLabel
        anchors {
            top : parent.top
            topMargin: parent.height / 6
            left: parent.left
        }
        height: parent.height / 10
        width: parent.width / 3

        Label {
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height

            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            horizontalAlignment: Text.AlignHCenter
        }
    }

    CloudRect {
        id: cloudRect1
        anchors {
            top: dateLabel.bottom
            left: parent.left
        }
        width: dateLabel.width
        height: parent.height*3/5
    }

    CloudRect {
        id: cloudRect2
        anchors {
            top: parent.top
            left: cloudRect1.right
            leftMargin: margin
        }
        height: parent.height*5/9
        width: cloudRect1.width
    }

    CloudRect {
        id: cloudRect3
        anchors {
            left: cloudRect2.right
            leftMargin: margin
            right: parent.right
            bottom: cloudRect2.bottom
        }
        height: parent.height*4/9
    }

    CloudRect {
        id: cloudRect4
        anchors {
            top: cloudRect2.bottom
            topMargin: margin * 2
            bottom: parent.bottom
            left: cloudRect1.right
            leftMargin: margin
        }
        width: parent.width * 2 / 9
    }

    CloudRect {
        id: cloudRect5
        anchors {
            top: cloudRect2.bottom
            bottom: parent.bottom
            left: cloudRect4.right
            topMargin: margin * 2
            leftMargin: margin
        }
        width: cloudRect4.width
    }
}
