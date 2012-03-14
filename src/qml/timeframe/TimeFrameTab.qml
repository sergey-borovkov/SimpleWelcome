import QtQuick 1.0


Rectangle {
    id: timeFrameTab
    width: parent.width
    height: 800
    clip: true
    //anchors.top: parent.top
    anchors.topMargin: 16
    property ListView lv: scene


    function startup() {

    }

    function prevMonth() {
//        if ( cloudItem.focus == true )
            scene.decrementCurrentIndex()
    }

    function nextMonth() {
//        if ( cloudItem.focus == true )
            scene.incrementCurrentIndex()
    }

    Component.onCompleted: startup();
    
    Text {
        text: activityListModel.count
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
    }



    ListView {
        id: scene
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
        model: activityListModel
        delegate: SceneDelegate {}
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
        highlightMoveDuration: 1000

        Keys.onLeftPressed: {
            console.log( "left key pressed 333..." )
            prevMonth()
        }
        visible: true
    }
    ListModel
    {
        id: activityListModel
    }

    Connections
    {
        target: activityProxy

        onNewList:
        {                        
            activityListModel.insert(index, {"activity": list})            
        }

        onListChanged:
        {
            activityListModel.set(index, {"activity": list})            
        }
    }


    Flickable
    {
        id: flickable
        anchors.fill: parent
        contentWidth: parent.width * 20
        contentHeight: parent.height
        Row {
            id: row


            Repeater
            {
                model: activityListModel
                delegate: SceneDelegate {}
            }
        }
        visible: false
    }



    TimeScale{
        id: timeScale
        anchors.verticalCenter: parent.verticalCenter
        height: 80
        width: parent.width
    }
    Connections{
        target: timeScale.list
        onCurrentIndexChanged: {
            activityProxy.setMonth(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber)
        }
    }
    ToolButton {
        id: prevButton
        imageUrl: "images/go-previous.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        onButtonClick: {
            console.log( "left button pressed..." )
            prevMonth()
        }
    }

    ToolButton {
        id: nextButton
        imageUrl: "images/go-next.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onButtonClick: {
            console.log( "right button pressed..." )
            nextMonth()
        }
    }

    Keys.onLeftPressed: {
        console.log( "left key pressed..." )
        prevMonth()
    }

    Keys.onRightPressed: {
        console.log( "right key pressed..." )
        nextMonth()
    }

    Keys.onEscapePressed: {
        Qt.quit()
    }

}
