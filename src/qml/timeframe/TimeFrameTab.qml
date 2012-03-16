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
    /*
    Text {
        text: activityListModel.count
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
    }
*/
    function getTimeScaleIndex( index )
    {
        var x = activityListModel.get(index).activity.getSetDate(0)
        var month = Qt.formatDateTime(x, "M")
        var year = Qt.formatDateTime(x, "yyyy")
        var i
        for (i = 0;  i < timeScale.list.count; i++)
        {
            var y = timeScale.model.get(i).year
            var z = timeScale.model.get(i).monthNumber
            //console.log("getTimeScaleIndex( index ): " + y + " " + z +" : " + year + " " + month)
            if ((year.toString() === y.toString()) && (month.toString() === z.toString()))
            {
                //  console.log("bingo")
                return i
            }
        }
        return -1
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
        snapMode: ListView.SnapOneItem
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
        highlightMoveDuration: 1000
        onCurrentIndexChanged: {
            /*
            if (currentIndex === 0) // Прокрутили на затычку
            {
                timeScale.list.incrementCurrentIndex()
            }else
            {

                var timeScaleIndex = getTimeScaleIndex(currentIndex)
                console.log("Scene index changed - " + currentIndex + " : " +  timeScaleIndex  )
                if (timeScaleIndex !== -1)
                    timeScale.list.currentIndex = timeScaleIndex
            }*/
        }



        Keys.onLeftPressed: {
            console.log( "left key pressed 333..." )
            prevMonth()
        }
        visible: true
    }
    /*
    Connections{
        target: scene

        onFlickStarted:
        {
            console.log("flick started" + scene.horizontalVelocity)
            if (scene.horizontalVelocity > 0) //Двигаем вправо
            {
                console.log("flick in right " + scene.currentIndex)
                if (scene.currentIndex === (scene.count -1))
                {
                    timeScale.list.decrementCurrentIndex()
                }
            } else //Двигаем влево
            {
                console.log("flick in letf " + scene.currentIndex)
                if (scene.currentIndex === 0)
                {
                    timeScale.list.incrementCurrentIndex()
                }
            }
        }

    }
*/


    ListModel
    {
        id: activityListModel
        //ListElement {activity: ""}
        //{"activity:" }
    }

    Connections
    {
        target: activityProxy

        onNewList:

       /* {
            console.log("+++++++++++++++ new item +++++++++++++")
            activityListModel.insert(index+1, {"activity": list})
            if ((index === 0) && (scene.currentIndex===0))
            {
                //if (scene.moving === false)
                    scene.positionViewAtIndex(index+1,ListView.Contain)
                /*else
                {
                    activityListModel.remove(0)
                    activityListModel.currentIndex = 0
                }




            if (scene.currentIndex === -1) //Был новый запрос на пустой месяц
            {
                var sceneIndex = getSceneIndex(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber)
                console.log("")
                console.log("new right item " + sceneIndex)
                console.log("")
                scene.currentIndex = sceneIndex
            }
            */

        {                        
            activityListModel.insert(index, {"activity": list})

        }

        onListChanged:
        {
            console.log("list changed - " + index)

            if(scene.currentIndex === index)
            {
                activityListModel.remove(index)
                activityListModel.insert(index, {"activity": list})
            }
            else
                activityListModel.set(index, {"activity": list})
//            {
  //              activityListModel.set(index, {"activity": list})
    //        }

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
        anchors.horizontalCenter: parent.horizontalCenter
        height: 80
        width: parent.width - 100
    }

    function getSceneIndex( year , month ) //Need future development
    {
        var i
        for (i =1;  i < scene.count; i++)
        {
            var x = Qt.formatDate( activityListModel.get(i).activity.getSetDate(0) , "M-yyyy")
            if (x.toString() === (month.toString() + '-' + year.toString()))
            {
                //console.log("bingo")
                return i
            }
            //console.log(Qt.formatDate(y, "M-yyyy"))
            //console.log(month + "-" + year)
        }
        return -1
    }

    Connections{
        target: timeScale.list
        onCurrentIndexChanged: {
            activityProxy.setMonth(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber - 1 )
            var sceneIndex = getSceneIndex(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber)
            console.log("Ts index changed - " + timeScale.list.currentIndex + " : "+ sceneIndex)

            if (sceneIndex !== -1)
            {
                console.log("change index in scene on " + sceneIndex)
                scene.currentIndex = sceneIndex

            }
            //scene.currentIndex = sceneIndex
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
