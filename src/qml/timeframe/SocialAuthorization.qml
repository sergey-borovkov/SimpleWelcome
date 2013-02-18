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
import ".."

PathView {
    id: pathView
    anchors.fill: parent

    property int itemCount: pathView.count
    property int itemWidth: 140

    path: Path {
        startX: (parent.width - itemWidth * (itemCount - 1)) / 2
        startY: height / 2
        PathLine {
            x: (parent.width + itemWidth * (itemCount + 1)) / 2
            y: height / 2
        }
    }

    interactive: false

    delegate: Item {
        width: authorizationItem.width
        height: authorizationItem.height

        Item {
            id: authorizationItem
            anchors.centerIn: parent
            width: childrenRect.width
            height: img.height + button.height + 10

            Image {
                // get icon for this plugin
                id: img
                width: 128
                height: 128
                z: -1

                smooth: true
                source: "image://plugin/" + name
            }

            Item {
                id: button
                anchors {
                    top: img.bottom
                    left: img.left
                    right: img.right
                    leftMargin: 5
                    rightMargin: 5
                }
                height: 28

                Image {
                    id: buttonImage
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: authorized ? 15 : 20
                    }
                    width: 22
                    height: 22

                    smooth: true
                    source: authorized ? "images/logout.png" : "images/login.png"
                }

                Text {
                    id: buttonText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: buttonImage.right
                        leftMargin: 5
                        right: parent.right
                    }

                    text: authorized ? i18n("Logout") : i18n("Login")
                    color: "white"
                    font.pointSize: 12
                }
            }

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true

                onEntered: authorizationItem.state = "hover"
                onExited:  authorizationItem.state = ""
                onPressed: authorizationItem.state = ""
                onReleased: authorizationItem.state = "hover"
                onClicked: {
                    if(authorized) {
                        socialProxy.logout(name)
                    }
                    else {
                        socialProxy.login(name)
                    }
                }
            }

            states: [
                State {
                    name: "hover"
                    PropertyChanges {
                        target: img
                        width: 134
                        height: 134
                    }

                    PropertyChanges {
                        target: buttonImage
                        width: 24
                        height: 24
                    }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation { properties: "width,height"; duration: 100}
                }
            ]
        }
    }
}
