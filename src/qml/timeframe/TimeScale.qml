import QtQuick 1.1

Item {
    id: timeScale
    property variant list: timeScaleList
    //property variant  model: monthModel

    function getMonthStr(index) {

        switch (index) {
        case 1:
            return i18n_JAN
        case 2:
            return i18n_FEB
        case 3:
            return i18n_MAR
        case 4:
            return i18n_APR
        case 5:
            return i18n_MAY
        case 6:
            return i18n_JUN
        case 7:
            return i18n_JUL
        case 8:
            return i18n_AUG
        case 9:
            return i18n_SEP
        case 10:
            return i18n_OCT
        case 11:
            return i18n_NOV
        case 12:
            return i18n_DEC
        }
        return "UND"
    }

    Component {
        id: monthDelegate
        Item {
            id: listItem
            width: 107
            height: 80
            Image {
                id: scaleImage
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                source: "images/month1.png"

            }
            Text {
                color: "white"
                id: monthLabel
                text: getMonthStr(month)// + ' ' + year
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                //anchors.bottomMargin: 1
                font.pixelSize: 15
            }
        }
    }
    Component {
        id: highlight
        Item{
            width: 107
            height: 32
            x: (timeScaleList.currentIndex === -1)? 0 : timeScaleList.currentItem.x
            y: (timeScaleList.currentIndex === -1)? 0 : timeScaleList.currentItem.y + timeScaleList.currentItem.height/2 - 16
            Image {
                id: activeLeft
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: "images/active-left.png"
            }
            Image {
                id: activeRight
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "images/active-right.png"
            }
            Image {
                anchors.left: activeLeft.right
                anchors.right: activeRight.left
                anchors.verticalCenter: parent.verticalCenter
                source: "images/active-center.png"
                smooth: true
            }
            Behavior on x { NumberAnimation{duration: 300 } }
        }
    }

    Text {
        id: yearLabel
        color: "white"
        anchors.left: parent.left
        anchors.right: listViewAnchor.left
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 15

        Connections {
            target: timeScaleList
            onCurrentIndexChanged: {
                yearLabel.text = timeScaleModel.getYear(timeScaleList.currentIndex)
            }
        }
    }

    function getListViewItemsCount() {
        var y = Math.floor((timeScale.width - 100)/ 107)
        return y
    }

    Item {
        id: listViewAnchor
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        clip: true
        width: getListViewItemsCount()*107

        Item {
            id: scaleBackground
            anchors.fill: parent
            Image {
                id: scaleLeft
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: "images/scale-left.png"
            }
            Image {
                id: scaleRight
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "images/scale-right.png"
            }
            Image {
                anchors.left: scaleLeft.right
                anchors.right: scaleRight.left
                anchors.verticalCenter: parent.verticalCenter
                source: "images/scale-center.png"
                smooth: true
            }
        }

        ListView {
            id: timeScaleList
            model: timeScaleModel
            delegate: monthDelegate
            highlight: highlight
            highlightFollowsCurrentItem: false
            boundsBehavior: Flickable.StopAtBounds
            //layoutDirection: Qt.RightToLeft
            anchors.fill: parent
            anchors.rightMargin: 3
            anchors.leftMargin: 3
            height: 80
            focus: true
            orientation: Qt.Horizontal
            snapMode: ListView.SnapToItem
            preferredHighlightBegin: 0; preferredHighlightEnd: 0

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

                        timeFrameTab.currentView.positionViewAtIndex(timeFrameTab.currentView.currentIndex, ListView.Center )
                    }
                }
            }
            Behavior on contentX {
                NumberAnimation { duration: 300 }
            }
        }
    }
}
