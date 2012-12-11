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

Item {
    property int margin: 10
    property int labelHeight: 20

    id: socialGalleryItem
    height: parent.height
    width: flow.childrenRect.width + 20

    Text {
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( date , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    AnimatedImage {
        id: waitIndicator
        source: "images/ajax-loader.gif"
        anchors.centerIn: parent
        visible: (size === 0) ? true : false
    }

    Item {
        id: itemFlow
        anchors.top: dateLabel.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        Flow {
            id: flow

            property real originalHeight: parent.height

            x: 5; y: 5
            height: originalHeight
            spacing: 10
            flow: Flow.TopToBottom

            Repeater {
                id: repeater
                model: socialDayModel.itemsModel(date)
                SocialGalleryItem { }
            }
        }
    }


    function getDelegateWidth( count ){
        if (count === 0)
        {
            galleryLister.startSearch(date,1)   //Start new search if null item becomes visible
            count++
        }
        var x = Math.ceil(count /3)
        return x
    }
    function printRoles()
    {
        console.log("date " + date)
        console.log("size " + size)
        return Math.ceil(size /3)
    }
}
