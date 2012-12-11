/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

Item {
    property int margin: 10
    property int labelHeight: 20

    id: galleryItem
    height: parent.height
    width: getDelegateWidth(size) * (parent.height - 2*margin - dateLabel.height)/3 + 2*margin

    function getDelegateWidth( count ) {
        if (count === 0) {
            count++
        }
        var x = Math.ceil(count /3)
        return x
    }

    Label {
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: (size === 0) ? Qt.formatDate( date , "MMM yyyy") : Qt.formatDate( date , "dd MMM yyyy")
    }

    GridView {
        id: itemGrid
        anchors.top: dateLabel.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: localDayModel.itemsModel(date)
        cellWidth: (parent.height - 2*margin - dateLabel.height) / 3 //-20
        cellHeight: (parent.height - 2*margin - dateLabel.height) / 3 -1//-20
        cacheBuffer: 0
        flow: GridView.TopToBottom
        interactive: false
        delegate: GalleryDayDelegate {}
    }
}
