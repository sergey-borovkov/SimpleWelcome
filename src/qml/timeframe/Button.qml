/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
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

Rectangle {
    id: button
    width: 100
    height: 38
    state: ""

    color: "transparent"
    border {
        color: "transparent"
        width: 1
    }
    radius: 5
    opacity: 0.8

    states: State {
        name: "selected"
        PropertyChanges { target: button; color: "#222" }
        PropertyChanges { target: button; border.color: "black" }
        PropertyChanges { target: button; opacity: 1 }
    }

}
