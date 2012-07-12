import QtQuick 1.1

Item
{
    id: timeScale
    property variant list: timeScaleList
    property variant  model: monthModel

    function getMonthStr(index)
    {

        switch (index) {
        case 0:
            return "JAN"
        case 1:
            return "FEB"
        case 2:
            return "MAR"
        case 3:
            return "APR"
        case 4:
            return "MAY"
        case 5:
            return "JUN"
        case 6:
            return "JUL"
        case 7:
            return "AUG"
        case 8:
            return "SEP"
        case 9:
            return "OCT"
        case 10:
            return "NOV"
        case 11:
            return "DEC"
        }
        return "UND"
    }

    Connections{
        target: nepomukSource
        onNewTSEntries: {
            //console.log("New Entry")
            monthModel.append( { month: getMonthStr(month-1), year: year, monthNumber: month })
        }
    }

    ListModel
    {
        id: monthModel        
    }

    Component{
        id: monthDelegate
        Item{
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

            Rectangle
            {

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
                text: qsTr(month)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
            }
        }
    }
    Component{
        id: highlight
        Row{
            x: timeScaleList.currentItem.x-3
            y: timeScaleList.currentItem.y
            width: timeScaleList.width/10 + 6  //siz
            height: 80
            Repeater{
                model: (timeScaleList.width/10+6)/12
                Item {
                    width: 12
                    height: 18

                    Rectangle{
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

    Text
    {
        id: yearLabel
        color: "white"
        anchors.left: parent.left
        anchors.right: listViewAnchor.left
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 15
        Connections
        {
            target: timeScaleList
            onCurrentIndexChanged: {
                yearLabel.text = monthModel.get(timeScaleList.currentIndex).year
            }
        }
    }
    function getListViewItemSize()
    {
        var x = timeScale.width / 11//size of 1 element
        var y = Math.ceil(x/12)
        return y*12
    }

    Item
    {
        id: listViewAnchor
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        clip: true
        width: getListViewItemSize()*10+6

        ListView
        {
            id: timeScaleList
            model: monthModel
            delegate: monthDelegate
            highlight: highlight
            highlightFollowsCurrentItem: false
            boundsBehavior: Flickable.StopAtBounds
            layoutDirection: Qt.RightToLeft
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
                onClicked:
                {
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
                        timeFrameTab.__year = monthModel.get(timeScale.list.currentIndex).year
                        timeFrameTab.__month = monthModel.get(timeScale.list.currentIndex).monthNumber - 1
                        timeFrameTab.currentDateChanged()
                        //scene.currentIndex = timeFrameTab.getTimeLineProperlyItem()

                        //set index on timeLine
                        timeLine.currentIndex = timeFrameTab.getTimeLineGalleryIndex()
                        timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Center )

                        //set index on gallery
                        galleryView.currentIndex = timeFrameTab.getTimeLineGalleryIndex()
                        galleryView.positionViewAtIndex(galleryView.currentIndex, ListView.Center )
                    }
                }
            }
            Behavior on contentX {
                NumberAnimation { duration: 300 }
            }

        }
        Rectangle{
            id: scale
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            height: 6
            color: "grey"
        }
    }
}
