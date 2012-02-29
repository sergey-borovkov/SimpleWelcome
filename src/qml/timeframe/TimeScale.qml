import QtQuick 1.0

Item
{
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

    function fillModel()
    {
        var currentDate = new Date()
        var currentMonth = currentDate.getMonth()
        var currentYear = currentDate.getFullYear()
        var i,j
        for (i = 1970; i <= currentYear; i++)
        {
            for(j = 0; j < 12; j++)
            {
                if ((i === currentYear) && (j > currentMonth))
                {
                    break
                }
                //if (j===0)
                  //  monthModel.append( { name: getMonthStr(j) +" " +i, year: i })
                //else
                    monthModel.append( { name: getMonthStr(j), year: i })
            }
        }
    }

    Component.onCompleted:
    {
        fillModel()
        timeScaleList.currentIndex = monthModel.count -1
        timeScaleList.positionViewAtIndex(timeScaleList.currentIndex, ListView.Contain)
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
            //state: (name ==="UND") ? "und" : "normal"
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
                height: 6
                width: parent.width
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
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
                id: monthLabel
                text: qsTr(name)
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
                Rectangle {
                    width: 12
                    height: 18
                    Rectangle{
                        //anchors.horizontalCenter: parent.horizontalCenter
                        //anchors.verticalCenter: parent.verticalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.left: parent.left
                        //anchors.leftMargin: -3
                        width: 6; height: parent.height
                        color: "grey"
                    }
                }
            }
            //Behavior on x { SpringAnimation { spring: 1; damping: 0.2 } }
            Behavior on x { NumberAnimation{duration: 300 } }
        }
    }

    Text
    {
        id: yearLabel
        anchors.left: parent.left
        anchors.right: listViewAnchor.left
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 15
        //width: parent.width/11
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
        var x = parent.width / 11//size of 1 element
        var y = Math.ceil(x/12)
        return y*12
    }

    Item
    {
        id: listViewAnchor
        //anchors.left: yearLabel.right
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

            //anchors.left: yearLabel.right
            anchors.fill: parent
            anchors.rightMargin: 3
            anchors.leftMargin: 3
            height: 80
            focus: true
            orientation: Qt.Horizontal
            //clip: true
            snapMode: ListView.SnapToItem
            preferredHighlightBegin: 0; preferredHighlightEnd: 0

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                //hoverEnabled: true
                onClicked:
                {
                    var mouseIndex = timeScaleList.indexAt(mouseX + timeScaleList.contentX, mouseY + timeScaleList.contentY)
                    if (mouseIndex !== -1)
                    {
                        timeScaleList.currentIndex = mouseIndex
                    }
                }
            }
            Behavior on contentX {
                NumberAnimation { duration: 300 }
            }

        }
    }

}
