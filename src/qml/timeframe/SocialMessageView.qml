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
    property alias view: msgView
    property alias scrollBar: msgScrollBar
    property alias socialMessage: socialMessage

    Flickable {
        id: msgView
        width: parent.width
        height: parent.height
        contentHeight: socialMessage.height
        contentWidth: socialMessage.width
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Text {
            id: socialMessage
            width: msgView.width
            anchors {
                bottomMargin: 3
                horizontalCenter: parent.horizontalCenter
            }

            onLinkActivated: {
                Qt.openUrlExternally(link)
            }

            wrapMode: Text.Wrap
            horizontalAlignment: truncated ? Text.AlignLeft : Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: (picture === "") ? message : ""
            color: "white"
            textFormat: Text.StyledText
            elide: Text.ElideRight
            maximumLineCount: {
                var h = msgView.parent.height - 10
                h -= (socialImage.height - 10) * socialImage.visible
                h -= (audioItem.height - 10) * audioItem.visible
                h -= (videoItem.height - 10) * videoItem.visible
                var lines = h / font.pixelSize / 1.5;
                return lines
            }
        }
    }

    ScrollBar {
        id: msgScrollBar
        flickable: msgView
        vertical: true
        hideScrollBarsWhenStopped: false
        visible: false
    }
}
