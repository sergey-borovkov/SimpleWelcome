/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Julia Mineeva <julia.mineeva@osinit.ru>
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

Text {
    anchors {
        verticalCenter: parent.verticalCenter
        leftMargin: 16
        rightMargin: 16
        topMargin: 8
    }

    maximumLineCount: 1
    clip: true
    elide: Text.ElideRight
    wrapMode: Text.Wrap

    font {
        pointSize: 14
        family: "Bitstream Vera Sans"
    }

    style: Text.Raised
    styleColor: "#000"
    color: "#eee"
}
