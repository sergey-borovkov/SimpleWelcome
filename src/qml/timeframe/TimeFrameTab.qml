import QtQuick 1.0


Item {
    id: timeFrameTab
    width: parent.width
    height: 800
    clip: true
    //anchors.top: parent.top
    anchors.topMargin: 16
    property ListView lv: scene

    property int __year: new Date().getFullYear()   //Current year
    property int __month: new Date().getMonth()     //Current month
    property bool __isSearching: false              //New search in process
    property bool direction: false  //true is - right direction; false - is left



//    function startup() {

//    }

    function prevMonth() {
        //        if ( cloudItem.focus == true )
        scene.decrementCurrentIndex()
    }

    function nextMonth() {
        //        if ( cloudItem.focus == true )
        scene.incrementCurrentIndex()
    }


    function getTimeLineProperlyItem() {
       var index = activityModel.getListIndex(__year, __month+1, direction)
       return index
    }

    //Component.onCompleted: startup();

//    function getTimeScaleIndex( index )
//    {
//        var x = activityModel.get(index).date
//        var month = Qt.formatDateTime(x, "M")
//        var year = Qt.formatDateTime(x, "yyyy")
//        var i
//        for (i = 0;  i < timeScale.list.count; i++)
//        {
//            var y = timeScale.model.get(i).year
//            var z = timeScale.model.get(i).monthNumber
//            if ((year.toString() === y.toString()) && (month.toString() === z.toString()))
//            {
//                //  console.log("bingo")
//                return i
//            }
//        }
//        return -1
//    }


    //Start new serch
    function currentDateChanged()
    {        
        //var date = new Date(__year, __month)
        activityProxy.startNewSearch(__year, __month, direction)
        __isSearching = true
        searchLabel.visible = true
    }

    //Start initial search
    Connections{
        target:tabListView
        onCurrentIndexChanged:{            
            if (tabListView.currentIndex === 3)
            {
                currentDateChanged()
                console.log("start initial search")
            }
        }
    }

    //On search finished
    Connections{
        target: activityProxy
        onFinished: {__isSearching = false ; console.log("search finished")
        searchLabel.visible = false}

    }


    Row {
            id: menuBar
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 16
            spacing: 0
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
                id: localDocs
                model: menuDocItems
                name: "My Local Documents"
                state: "current"
                onSelectedIndexChanged: {
                    console.debug( selectedText + ", " + menuDocItems.get( selectedIndex ).itemText )
                }
                onClicked: {
                   console.debug( "My Local Documents clicked" )
                    state = "current"
                    socialNetworks.state = ""

                }
            }

            DropListBox {
                id: socialNetworks
                model: menuSocialItems
                name: "Social networking sites"
                onSelectedIndexChanged: {
                    console.debug( selectedText + ", " + menuSocialItems.get( selectedIndex ).itemText )
                }

                onClicked: {
                    console.debug( "Social networking sites clicked" )
                    state = "current"
                    localDocs.state = ""
                }
            }
        }

    Rectangle {
        id: separator
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: menuBar.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 2
        anchors.bottomMargin: 10
        height: 3
        color: "grey"
        radius: 1

    }

    function getTSCurrentDate()
    {
        var date = new Date(timeScale.model.get(timeScale.list.currentIndex).year, timeScale.model.get(timeScale.list.currentIndex).monthNumber - 1, 1)
        return date
    }

    function getTSIndex(year, monthNumber) //get index of date in TimeScale
    {        
        var i
        for (i = 0; i < timeScale.model.count; i++ )
        {
            if ((year === timeScale.model.get(i).year) && (monthNumber === timeScale.model.get(i).monthNumber - 1 ))
                return i
        }
        return -1
    }

/*
    function getFlickIndex()
    {
        console.log(getTSCurrentDate())
        if (direction) //get  first day in mounth
        {

        } else //get last day in mounth
        {

        }
        return scene.count -1
    }

*/
    Text
    {
        id: searchLabel
        color: "white"
        anchors.top: separator.bottom
        anchors.left: parent.left
        text: "searching in process "
        visible: false
        z: 1000
    }

    Binding
    {
        target: timeFrameTab
        property: __isSearching
        value:  searchLabel.visible

    }

    /*
    Connections{
        target: timeFrameTab
        onPropertyChanged: searchLabel.visible = __isSearching
    }
*/
    ListView {
        id: scene

        //property int __oldIndex: currentIndex

        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height - menuBar.height - separator.height
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
        onCurrentIndexChanged:
        {

            var date = activityModel.getDateOfIndex(scene.currentIndex)
            var tsDate = getTSCurrentDate()
            if (date.getTime() === tsDate.getTime())
            {                
                //__oldIndex = scene.currentIndex
                return
            }

            if (date.getTime() > tsDate.getTime())
                direction = true
            else
                direction = false

            //start new search
            timeFrameTab.__year = date.getFullYear()
            timeFrameTab.__month = date.getMonth()
            timeFrameTab.currentDateChanged()

            //check TS index
            if (date.getTime() !== tsDate.getTime())
                timeScale.list.currentIndex = getTSIndex(date.getFullYear(), date.getMonth())

            //__oldIndex = scene.currentIndex
        }
        onCountChanged:
        {
            console.log("new items added, scene count: " +  scene.count)
            scene.positionViewAtIndex(scene.currentIndex, ListView.Contain)            
        }
/*
        onCurrentIndexChanged:
        {
            if (currentIndex===0)
                timeScale.list.decrementCurrentIndex();
            if (currentIndex===count)
                timeScale.list.incrementCurrentIndex();
        }
*/
        /*
        Keys.onLeftPressed: {
            console.log( "left key pressed 333..." )
            prevMonth()
        }
        */
        visible: true
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

    ToolButton {
        id: prevButton
        width: 60
        imageUrl: "images/go-previous.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        z:100
        onButtonClick: {
            galleryView.decrementCurrentIndex()

            /*
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
            */
        }
    }

    ToolButton {
        id: nextButton
        width: 60
        imageUrl: "images/go-next.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        z:100
        onButtonClick: {
            galleryView.incrementCurrentIndex()
            /*
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
            */
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

 /*
    State {
        name: "local"
        PropertyChanges {
            target: scene
            delegate: SceneDelegate {}
        }
    }
    State {
        name: "social"
        PropertyChanges {
            target: scene
            delegate: SceneDelegate {}
        }
    }
    */
    Button {
        id: stateChangeButton
        width: 50
        height: 30
        anchors.bottom: separator.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 20
        border.color: "black"

        ButtonText {
            id: stateChangeButtonText
            text: "<--"
            anchors.left: parent.left
        }

        MouseArea{
            id: stateTestButtonMouseArea
            anchors.fill: parent
            onClicked: {
                console.log("State button clicked " + timeFrameTab.state)
                if ( timeFrameTab.state === "" ) {
                    timeFrameTab.state = "gallery"
                } else {
                    timeFrameTab.state = ""
                }
            }
        }
    }

    ListView {
        id: galleryView
        anchors.top: separator.bottom
        anchors.bottom: timeScale.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        visible: false
        highlightFollowsCurrentItem: true
        model: galleryModel
        delegate: GalleryDelegate { }
        /*
        delegate: Item {
            width: 200
            height: parent.height
            Rectangle{
                id: back
                anchors.fill: parent
                color: "red"
            }
            Text {
                id: label
                anchors.fill: parent
                text: Qt.formatDate( date , "dd-M-yyyy")
                color: "white"
            }

        }
        */
        orientation: ListView.Horizontal
    }

    state: "gallery"

    states: [

        State {
            name: "gallery"
            AnchorChanges{
                target: timeScale

                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
                //anchors.right:   timeFrameTab.right

            }
            PropertyChanges {
                target: scene
                visible : false
            }
            PropertyChanges {
                target: galleryView
                visible : true
            }            
            PropertyChanges {
                target: menuBar
                visible : false
            }
        }
    ]
    transitions: Transition {
         AnchorAnimation {duration: 500}
     }

}
