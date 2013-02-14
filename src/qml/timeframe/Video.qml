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
import Private 0.1
import ".."

Item {
    id: video
    anchors.fill: image
    anchors.margins: 10

    property alias loader: videoLoader
    property alias mouseArea: videoMouseArea
    property string url

    function load(){
        videoMouseArea.hoverEnabled = true
        videoLoader.sourceComponent = videoDelegate
        videoLoader.item.source = url
        videoLoader.item.play()
    }

    Loader {
        id: videoLoader
        anchors.fill: parent
    }

    Image {
        id: controlImage
        anchors.centerIn: parent
        source: "images/pause-empty.png"
        opacity: 0
        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }
    }

    MouseArea {
        id: videoMouseArea
        anchors.fill: parent
        onEntered: {
            controlImage.opacity = 1
        }
        onExited: {
            //Prevent hiding icon when video is paused
            if (!videoLoader.item.playing() && videoLoader.item.ready) {
                return
            }
            controlImage.opacity = 0
        }
        onClicked: {
            if (videoLoader.item.playing()) {
                controlImage.source = "images/play-empty.png"
            }
            else {
                controlImage.source = "images/pause-empty.png"
            }
            videoLoader.item.playOrPause()
        }
    }

    Component {
        id: videoDelegate
        Video
        {
            id: videoItem
        }
    }

}
