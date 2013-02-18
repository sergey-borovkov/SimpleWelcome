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

Column {
    spacing: 5

    Item {
        id: imageBackground
        width: itemGrid.cellWidth - 20
        height: itemGrid.cellHeight - 40

        Image {
            id: image
            anchors.centerIn: parent

            source: "image://gallery/" + url + "%" + image
            sourceSize.width: parent.width - 4
            sourceSize.height: parent.height - 4
            cache: false

            Component.onDestruction: {
                galleryPreviewGenerator.cancel(image)
            }

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
        }

        Loader {
            id: video
            anchors {
                fill: image
                margins: 10
            }
        }

        MouseArea {
            id: imageMouseArea
            anchors.fill: parent

            hoverEnabled: true

            onClicked: {
                if(video.status === Loader.Ready) {
                    video.item.clicked()
                    return
                }

                if (type === "Video") { //Start video preview
                    video.source = "Video.qml"
                    video.item.url = url
                    video.item.load()
                    image.visible = false
                    return
                }

                Qt.openUrlExternally(url)
            }

            onEntered: {
                if(video.status === Loader.Ready)
                    video.item.entered()
            }

            onExited: {
                if(video.status === Loader.Ready)
                    video.item.exited()
            }
        }


        Image {
            id: klook
            anchors {
                top: image.top
                right: image.right
            }

            source: "image://generalicon/appicon/klook"
            sourceSize {
                width: 24
                height: 24
            }

            visible: imageMouseArea.containsMouse

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    var p = image.mapToItem(null, 0, 0)
                    klookProcess.show(p.x,
                                      p.y,
                                      image.width,
                                      image.height, url)
                }
            }
        }
    }

    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width

        text:  url.replace(/^.*[\\\/]/, '')

        style: Text.Raised
        styleColor: "#000"
        color:  "white"
        font {
            family: "Bitstream Vera Sans"
            pointSize: constants.iconTextSize
        }

        elide: Text.ElideMiddle
        maximumLineCount: 3
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
