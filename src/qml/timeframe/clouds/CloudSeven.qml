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
    id: cloudSeven

    property date cloudDate
    property variant model
    property int margin: 10

    Item {
        anchors {
            fill: parent
            topMargin: parent.height / 10
            bottomMargin: parent.height / 10
        }

        Item {
            id: dateLabel
            anchors {
                top : parent.top
                topMargin: parent.height / 6
                left: parent.left
            }
            height: parent.height / 10
            width: parent.width / 4

            Label {
                anchors.centerIn: parent
                width: 100
                height: dateLabel.height

                text: Qt.formatDate(cloudDate , "dd MMM yyyy")
            }
        }

        CloudRect {
            id: cloudRect1
            anchors {
                top: dateLabel.bottom
                left: parent.left
            }
            width: dateLabel.width
            height: parent.height * 3 / 5
        }

        CloudRect {
            id: cloudRect2
            anchors {
                top: parent.top
                left: cloudRect1.right
                leftMargin: margin
            }
            height: parent.height * 5 / 9
            width: cloudRect1.width
        }

        CloudRect {
            id: cloudRect3
            anchors {
                left: cloudRect2.right
                leftMargin: margin
                bottom: cloudRect2.bottom
            }
            width: parent.width / 5
            height: parent.height * 4 / 9
        }

        CloudRect {
            id: cloudRect7
            anchors {
                top: cloudRect1.top
                bottom: cloudRect1.bottom
                left: cloudRect3.right
                leftMargin: margin
            }
            width: cloudRect1.width
        }

        CloudRect {
            id: cloudRect4
            anchors {
                top: cloudRect2.bottom
                bottom: parent.bottom
                left: cloudRect1.right
                topMargin: margin * 2
                leftMargin: margin
            }
            width: (cloudRect1.width + cloudRect3.width - margin) / 3
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

        CloudRect {
            id: cloudRect6
            anchors {
                top: cloudRect2.bottom
                bottom: parent.bottom
                left: cloudRect5.right
                right: cloudRect7.left
                topMargin: margin*2
                leftMargin: margin
                rightMargin: margin
            }
            width: cloudRect4.width
        }
    }
}
