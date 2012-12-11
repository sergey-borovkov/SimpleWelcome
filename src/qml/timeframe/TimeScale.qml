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
    id: timeScale

    property variant list: timeScaleList
    property variant monthsStrs: [ i18n("JAN"), i18n("FEB"), i18n("MAR"), i18n("APR"), i18n("MAY"),
        i18n("JUN"), i18n("JUL"), i18n("AUG"), i18n("SEP"), i18n("OCT"), i18n("NOV"), i18n("DEC") ]

    function getMonthStr(index) {
        if (index > 0 && index <= 12)
            return monthsStrs[index - 1]
        return i18n("UND")
    }

    function getListViewItemsCount() {
        var y = Math.floor((timeScale.width - yearLabel.paintedWidth) / 110)
        return y
    }

    Component {
        id: monthDelegate

        Item {
            id: listItem
            width: 110
            height: 80

            Image {
                id: scaleImage
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }

                source: "images/month1.png"
            }

            Text {
                id: monthLabel
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }

                color: "white"
                text: getMonthStr(month)// + ' ' + year
                font.pixelSize: 15
            }
        }
    }

    Component {
        id: highlight

        Item {
            width: 112
            height: 32
            x: (timeScaleList.currentIndex === -1)? 0 : timeScaleList.currentItem.x -1
            y: (timeScaleList.currentIndex === -1)? 0 : timeScaleList.currentItem.y + timeScaleList.currentItem.height/2 - 16

            Image {
                id: activeLeft
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                source: "images/active-left.png"
            }

            Image {
                id: activeRight
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                source: "images/active-right.png"
            }

            Image {
                anchors {
                    left: activeLeft.right
                    right: activeRight.left
                    verticalCenter: parent.verticalCenter
                }

                source: "images/active-center.png"
                smooth: true
            }

            Behavior on x { NumberAnimation{duration: 300 } }
        }
    }

    Text {
        id: yearLabel
        anchors {
            left: parent.left
            right: listViewAnchor.left
            verticalCenter: parent.verticalCenter
        }

        color: "white"
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 15

        Connections {
            target: timeScaleList
            onCurrentIndexChanged: {
                yearLabel.text = timeScaleModel.getYear(timeScaleList.currentIndex)
            }
        }
    }

    Item {
        id: listViewAnchor
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        height: parent.height
        width: getListViewItemsCount()*110

        clip: true

        Item {
            id: scaleBackground
            anchors.fill: parent

            Image {
                id: scaleLeft
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                source: "images/scale-left.png"
            }

            Image {
                id: scaleRight
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                source: "images/scale-right.png"
            }
            Image {
                anchors {
                    left: scaleLeft.right
                    right: scaleRight.left
                    verticalCenter: parent.verticalCenter
                }

                source: "images/scale-center.png"
                smooth: true
            }
        }

        ListView {
            id: timeScaleList
            anchors {
                fill: parent
                rightMargin: 3
                leftMargin: 3
            }
            height: 80

            model: timeScaleModel
            delegate: monthDelegate
            highlight: highlight
            highlightFollowsCurrentItem: false
            boundsBehavior: Flickable.StopAtBounds
            focus: true
            orientation: Qt.Horizontal
            snapMode: ListView.SnapToItem
            preferredHighlightBegin: 0
            preferredHighlightEnd: 0

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onClicked: {
                    var mouseIndex = timeScaleList.indexAt(mouseX + timeScaleList.contentX, mouseY + timeScaleList.contentY)
                    var oldIndex = timeScaleList.currentIndex
                    if ((mouseIndex !== -1) && (oldIndex !== mouseIndex))
                    {
                        //change current index
                        timeScaleList.currentIndex = mouseIndex

                        var date = new Date(timeScaleModel.getYear(timeScaleList.currentIndex), timeScaleModel.getMonth(timeScaleList.currentIndex) - 1, 1)

                        if (isSocial)
                            timeFrameTab.currentView.currentIndex = socialDayModel.getIndexByDate(date)
                        else
                            timeFrameTab.currentView.currentIndex = localDayModel.getIndexByDate(date)

                        timeFrameTab.currentView.positionViewAtIndex(timeFrameTab.currentView.currentIndex, ListView.Center)
                    }
                }
            }

            Behavior on contentX {
                NumberAnimation { duration: 300 }
            }
        }
    }
}
