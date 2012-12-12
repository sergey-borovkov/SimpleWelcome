/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
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

Rectangle {
    id: commentBox

    property alias edit: edit
    property alias userPhoto: userPhoto

    signal send(string comment)

    color: "#172634"
    radius: 10
    clip: true

    Image {
        id: userPhoto
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: 5
        }
        width: 55

        fillMode: Image.PreserveAspectFit
    }

    Rectangle {
        id: editBorder
        anchors {
            left: userPhoto.right
            right: parent.right
            top : parent.top
            bottom: parent.bottom
            leftMargin: 5
            topMargin: 5
            bottomMargin: 5
            rightMargin:  5
        }
        color: "white"
        border.color: "#BDC7D8"

        Flickable {
            id: flick
            anchors {
                fill: parent
                topMargin: 3
                leftMargin: 3
                rightMargin: 3
                bottomMargin: 3
            }

            contentWidth: edit.paintedWidth
            contentHeight: edit.paintedHeight
            clip: true

            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: edit
                width: flick.width
                height: flick.height

                focus: true
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

                function trim(str) {
                    return str.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
                }

                Keys.onPressed: {
                    if (edit.text === i18n("Write comment...")) {
                        edit.color = "black"
                        edit.text = ""
                        edit.cursorPosition = 0
                    }
                    if (event.key === Qt.Key_Return) {
                        // add "shift+enter" process
                        if (event.modifiers & Qt.ShiftModifier) {
                            return;
                        }

                        var comment = edit.text
                        // exit if empty comment
                        if (comment.length != 0 && comment.replace(/\s/g,"") != "") {

                            if (comment.length > 4096)
                                comment = comment.slice(0, 4096)

                            // trim spaces from start and end of string
                            comment = trim(comment)
                            commentBox.send(comment)
                        }
                        edit.color = "grey"
                        edit.text = i18n("Write comment...")
                        event.accepted = true
                    }
                }
            }
        }
    }
}

