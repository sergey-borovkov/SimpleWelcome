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
/*
    Connections {
        target: nepomukSource
        onNewTSEntries: {
            monthModel.append( { month: getMonthStr(month-1), year: year, monthNumber: month })
        }
    }

    ListModel {
        id: monthModel
    }
*/
    Component {
        id: monthDelegate
        Item {
            id: listItem
            width: timeScaleList.width/10
            height: 80

            Rectangle
            {
                anchors.left: parent.left
                anchors.leftMargin: -3
                anchors.verticalCenter: parent.verticalCenter
                width: 6
                height: 30
                color: "grey"
            }

            Rectangle {

                anchors.right: parent.right
                anchors.rightMargin: -3
                anchors.verticalCenter: parent.verticalCenter
                width: 6
                height: 30
                color: "grey"
            }

            Text {
                color: "white"
                id: monthLabel
                text: getMonthStr(month)// + ' ' + year
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
            }
        }
    }
    Component {
        id: highlight

        Row {
            x: (timeScaleList.currentIndex === -1)? 0 : timeScaleList.currentItem.x-3
            y: (timeScaleList.currentIndex === -1)? 0 :timeScaleList.currentItem.y
            width: timeScaleList.width/10 + 6  //siz
            height: 80
            Repeater {
                model: (timeScaleList.width/10+6)/12

                Item {
                    width: 12
                    height: 18

                    Rectangle {
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.left: parent.left
                        width: 6; height: parent.height
                        color: "grey"
                    }
                }
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

    function getListViewItemSize() {
        var x = timeScale.width / 11//size of 1 element
        var y = Math.ceil(x / 12)
        return y * 12
    }

    Item {
        id: listViewAnchor
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        clip: true
        width: getListViewItemSize()*10+6

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
                        //start new search here
                        if (mouseIndex > oldIndex)
                            timeFrameTab.direction = false // moving left
                        else
                            timeFrameTab.direction = true  // moving right
                        //timeFrameTab.__year = monthModel.get(timeScale.list.currentIndex).year
                        timeFrameTab.__year = timeScaleModel.getYear(timeScaleList.currentIndex)
                        //timeFrameTab.__month = monthModel.get(timeScale.list.currentIndex).monthNumber - 1
                        timeFrameTab.__month = timeScaleModel.getMonth(timeScaleList.currentIndex) - 1
                        timeFrameTab.currentDateChanged()
                        //scene.currentIndex = timeFrameTab.getTimeLineProperlyItem()

                        if ((timeFrameTab.state === "") || (timeFrameTab.state === "gallery"))
                        {
                            //set index on timeLine

                            timeLine.currentIndex = timeFrameTab.getTimeLineIndex()
                            timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Center )

                            //set index on gallery
                            galleryView.currentIndex = timeLine.currentIndex
                            galleryView.positionViewAtIndex(galleryView.currentIndex, ListView.Center )
                        }
                        else if ((timeFrameTab.state === "social") || (timeFrameTab.state === "socialGallery"))
                        {
                            //set index on socialTimeLine

                            socialTimeLine.currentIndex = timeFrameTab.getSocialTimeLineIndex()
                            socialTimeLine.positionViewAtIndex(socialTimeLine.currentIndex, ListView.Center )

                            //set index on social gallery
                            socialGalleryView.currentIndex = socialTimeLine.currentIndex
                            socialGalleryView.positionViewAtIndex(socialGalleryView.currentIndex, ListView.Center )
                        }
                    }
                }
            }
            Behavior on contentX {
                NumberAnimation { duration: 300 }
            }
        }

        Rectangle {
            id: scale
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            height: 6
            color: "grey"
        }
    }
}
