/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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
//import Private 0.1
import ".."

Item {
    id: cloudRect

    property string url
    property string type
    property alias image: image

    Connections {
        target: localDayModel.itemsModel(date)
        onGotThumbnail: {
            if(path === url) {
                var oldSource = image.source
                image.source = ""
                image.source = oldSource
            }
        }
    }

    Item {
        id: imageWrapper
        anchors.horizontalCenter: parent.horizontalCenter
        width: cloudRect.width
        height: cloudRect.height - fileName.font.pixelSize - 10

        Image {
            id: image
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            y: imageWrapper.y + (imageWrapper.height - height) / 2 - fileName.font.pixelSize + 20

            fillMode: Image.PreserveAspectFit

            sourceSize {
                width: parent.width
                height: parent.height
            }

            cache: false
        }
        Loader {
            id: video
            anchors { fill: image; margins: 10 }
        }
    }
    Label {
        id: fileName
        anchors.horizontalCenter: parent.horizontalCenter
        y: image.y + image.paintedHeight
        width: parent.width

        text: url.replace(/^.*[\\\/]/, '')
        elide: Text.ElideMiddle
        font.bold: false
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            // check of item type
            if (type !== "Video") {
                Qt.openUrlExternally(url)
                return
            }
            image.visible = false
            mouseArea.enabled = false
            video.source = "../Video.qml"
            video.item.url = url
            video.item.load()
        }
    }
}
