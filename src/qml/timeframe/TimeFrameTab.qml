import QtQuick 1.0


Item {
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

    function getTimeScaleIndex( index )
    {
        var x = activityModel.get(index).date
        var month = Qt.formatDateTime(x, "M")
        var year = Qt.formatDateTime(x, "yyyy")
        var i
        for (i = 0;  i < timeScale.list.count; i++)
        {
            var y = timeScale.model.get(i).year
            var z = timeScale.model.get(i).monthNumber
            if ((year.toString() === y.toString()) && (month.toString() === z.toString()))
            {
                //  console.log("bingo")
                return i
            }
        }
        return -1
    }


    Row {
            id: menuBar
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottomMargin: 16
            spacing: 40
            z: 100

            ListModel {
                id: menuDocItems
                ListElement { itemText: "All" }
                ListElement { itemText: "Photo" }
                ListElement { itemText: "Video" }
                ListElement { itemText: "Documents" }
            }

            ListModel {
                id: menuSocialItems
                ListElement { itemText: "All" }
                ListElement { itemText: "Facebook" }
                ListElement { itemText: "Twitter" }
                ListElement { itemText: "V Kontakte" }
            }

            DropListBox {
                model: menuDocItems
                name: "My Local Documents"
                onSelectedIndexChanged: {
                    console.debug( selectedText + ", " + menuDocItems.get( selectedIndex ).itemText )
                }
            }

            DropListBox {
                model: menuSocialItems
                name: "Social networking sites"
                onSelectedIndexChanged: {
                    console.debug( selectedText + ", " + menuSocialItems.get( selectedIndex ).itemText )
                }
            }
        }

    ListView {
        id: scene
        anchors.top: menuBar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height - menuBar.height
        model: activityModel
        delegate: SceneDelegate {}
        focus: true
        orientation: Qt.Horizontal
        snapMode: ListView.SnapOneItem
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        clip: true
        highlightMoveDuration: 1000

        Keys.onLeftPressed: {
            console.log( "left key pressed 333..." )
            prevMonth()
        }
        visible: true
    }
/*
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
                model: activityModel
                delegate: SceneDelegate {}
            }
        }
        visible: false
    }
*/
    TimeScale{
        id: timeScale
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: 80
        width: parent.width - 100
    }

    function getSceneIndex( year , month ) //Need future development
    {
        var i
        for (i = 0; i < scene.count; i++)
        {
            var x = Qt.formatDate( activityModel.get(i).date , "M-yyyy")
            if (x.toString() === (month.toString() + '-' + year.toString()))
            {                
                return i
            }
        }
        return -1
    }

    Connections{
        target: timeScale.list
        onCurrentIndexChanged: {
            activityProxy.setMonth(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber - 1 )
            var sceneIndex = getSceneIndex(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber)

            if (sceneIndex !== -1)
            {
                console.log("change index in scene on " + sceneIndex)
                scene.currentIndex = sceneIndex
            }
        }
    }

    ToolButton {
        id: prevButton
        width: 60
        imageUrl: "images/go-previous.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        onButtonClick: {
            console.log( "left button pressed..." )
            if (scene.currentIndex === 0)
            {
                //timeScale.list.decrementCurrentIndex()
                timeScale.list.incrementCurrentIndex()
            }
            else
            {
                scene.decrementCurrentIndex()
            }
        }
    }

    ToolButton {
        id: nextButton
        width: 60
        imageUrl: "images/go-next.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onButtonClick: {
            console.log( scene.currentIndex + "  " + scene.count)
            if (scene.currentIndex === (scene.count -1))
            {
                //timeScale.list.incrementCurrentIndex()
                timeScale.list.decrementCurrentIndex()
                console.log("index")
            }
            else
            {
                scene.incrementCurrentIndex()
            }
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
