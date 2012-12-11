/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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
import "../"

Item{
    id: cloudNone
    property date cloudDate
    property variant model

    Item {
        anchors.centerIn: parent
        width: parent.width
        height: 200
        Label{
            id: dateLabel
            anchors.top : parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 60
            text: Qt.formatDate( cloudDate , "MMM yyyy")
        }
        AnimatedImage {
            id: waitIndicator
            anchors.top: dateLabel.bottom
            source: "images/ajax-loader.gif"
            anchors.centerIn: parent
        }
    }
}
