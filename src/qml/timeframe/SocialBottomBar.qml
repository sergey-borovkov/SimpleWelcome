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
    id: bottomLine

    property alias likesCount : likesCountText.text
    property alias commentsCount:  commentsCountText.text
    property variant isLiked: 0
    property bool commentsShow: false

    property alias commentsShowText: commentsShowAreaText

    signal sendLikeClicked
    signal showCommentsClicked

    Item {
        id: commentsCountArea
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 5
        }
        width: 40
        height: 22

        Image {
            id: commentIcon
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            width: 22
            height: 22

            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "images/comment.png"
        }

        Text {
            id: commentsCountText
            anchors {
                left: commentIcon.right
                verticalCenter: parent.verticalCenter
                leftMargin: 5
            }

            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }
    }

    Item {
        id: likesCountArea
        anchors {
            verticalCenter: parent.verticalCenter
            left: commentsCountArea.right
            leftMargin: 5
        }
        width: 40
        height: 22

        Image {
            id: bottomLikeIcon
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            width: 22
            height: 22

            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "images/like.png"
        }

        Text {
            id: likesCountText
            anchors {
                left: bottomLikeIcon.right
                verticalCenter: parent.verticalCenter
                leftMargin: 5
            }

            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }
    }

    Item {
        id: likeItemArea
        anchors {
            right:  parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        height: parent.height
        width: 100

        state: (isLiked === 1) ? "liked" : ""

        Text {
            id: likesText
            anchors.fill: parent

            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: i18n("Like")
            color: "white"
        }

        MouseArea {
            id: likeSendArea
            anchors.fill: parent

            hoverEnabled: true

            onEntered: likesText.font.bold = true
            onExited: likesText.font.bold = false
            onClicked: sendLikeClicked()
        }

        states: [
            State {
                name: "liked"
                PropertyChanges { target: likesText; text: i18n("Unlike") }
            }
        ]
    }

    Item {
        id: commentsShowArea
        anchors.centerIn: parent
        width: 140
        height: parent.height

        Image {
            anchors  {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }
            source: "images/arrow.png"
        }

        Text {
            id: commentsShowAreaText
            anchors {
                fill: parent
                leftMargin: 10
                rightMargin: 10
            }

            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            text: (commentsShow) ? i18n("Hide comments") : i18n("Show comments")
        }

        Image {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }
            source: "images/arrow.png"
        }

        MouseArea{
            anchors.fill: parent

            hoverEnabled: true

            onEntered: commentsShowAreaText.font.bold = true
            onExited: commentsShowAreaText.font.bold = false
            onClicked: showCommentsClicked()
        }
    }
}
