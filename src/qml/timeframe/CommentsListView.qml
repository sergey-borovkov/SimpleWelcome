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
    property alias model: commentsListView.model
    property alias view: commentsListView

    Component {
        id: commentsDelegate

        Item {
            width: 354
            height: ((textField.paintedHeight + nameField.paintedHeight) > userPhoto.height) ? textField.paintedHeight + nameField.paintedHeight : 55

            Image {
                id: userPhoto
                width: 55
                anchors {
                    left: parent.left
                    top: parent.top
                    rightMargin: 5
                }

                fillMode: Image.PreserveAspectFit
                source: fromPictureUrl
            }

            Text {
                id: nameField;
                anchors {
                    left: userPhoto.right
                    top: parent.top
                    leftMargin: 10
                }

                text: from
                color: "grey"
            }

            Text {
                id: textField;
                anchors {
                    left: userPhoto.right
                    top: nameField.bottom
                    leftMargin: 10
                }
                width: parent.width - userPhoto.width - 40

                text: messageText
                color: "white"
                wrapMode: Text.Wrap
            }
        }
    }

    ListView {
        id: commentsListView
        anchors.fill: parent

        property string parentId: ""
        property string pluginName: ""

        delegate: commentsDelegate
        clip: true

        ScrollBar{
            id: scrollBar
            flickable: commentsListView
            vertical: true
            hideScrollBarsWhenStopped: false
        }
    }
}
