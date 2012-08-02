import QtQuick 1.1

Item {
    id: timeFrameTab
    width: parent.width
    //height: 800
    clip: true
    anchors.topMargin: 16
    //property ListView lv: timeLine

    property int __year: new Date().getFullYear()   //Current year
    property int __month: new Date().getMonth()
    property int day: new Date().getDate()
    property bool __isSearching: false              //New search in process
    property bool direction: false  //true is - right direction; false - is left

    //    function getTimeLineProperlyItem() {
    //        var index = activityModel.getListIndex(__year, __month+1, direction)
    //        return index
    //    }

    function getTimeLineGalleryIndex() {
        var index = localDayModel.getIndexByDate(__year, __month+1, direction)
        //console.log("getIndexByDate: " + index)
        return index
    }


    //Start new serch
    function prevMonth() {
        timeLine.decrementCurrentIndex()
    }

    function nextMonth() {
        timeLine.incrementCurrentIndex()
    }

    //Start new serch
    function currentDateChanged()
    {
        var d = new Date(__year, __month, day )
        //activityProxy.startSearch(d, direction)
        activityProxy.startSearch(d, true)
        __isSearching = true
        //searchLabel.visible = true
       // if (timeFrameTab.state ==="gallery" )
         //   timeFrameTab.state = "gallerySearch"
    }

    //Start initial search
    Connections {
        target:tabListView
        onCurrentIndexChanged:{
            if (tabListView.currentIndex === 3) {
                currentDateChanged()
                tabListView.interactive = false
            }
        }
    }

    //On search finished
    Connections {
        target: activityProxy
        onFinished: {
            __isSearching = false
            //console.log("search finished")
            //searchLabel.visible = false
           // if (timeFrameTab.state === "gallerySearch")
             //   timeFrameTab.state = "gallery"
            //else if (timeFrameTab.state === "timeLineSearch")
            //  timeFrameTab.state = ""

        }
    }

    Row {
        id: menuBar
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
            Component.onCompleted: {
                append({ "itemText": "All"})
                var count = socialProxy.count();
                for(var i = 0; i < count; i++)
                    append({ "itemText": socialProxy.name(i) })
                append({ "itemText": "Manage networks"})
            }
        }

        DropListBox {
            id: localDocs
            model: menuDocItems
            name: "My Local Documents"
            state: "current"
            onSelectedIndexChanged: {
                console.log( selectedText + ", " + menuDocItems.get( selectedIndex ).itemText )
                activityProxy.changeType(selectedText)
                timeScaleModel.setFilter(selectedText)
                localDayModel.setFilter(selectedText)
            }
            onClicked: {
                console.log(selectedText + " " + selectedIndex)
                state = "current"
                timeFrameTab.state = ""
                socialNetworks.state = ""
            }
        }

        DropListBox {
            id: socialNetworks
            model: menuSocialItems
            name: "Social networking sites"
            property int __wasSearching: 0
            onSelectedIndexChanged: {
                if(selectedText == "Manage networks")
                    timeFrameTab.state = "socialauthorization"
                else {
                    timeFrameTab.state = "socialgallery"
                    socialProxy.startSearch()
                }

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
/*
    function getTSCurrentDate() {
        var date = new Date(timeScaleModel.getYear(timeScale.list.currentIndex), timeScaleModel.getMonth(timeScale.list.currentIndex).monthNumber, 1)
        return date
    }

*/
    //get index of date in TimeScale
    function getTSIndex(year, monthNumber) {
        var i
        for (i = 0; i < timeScaleModel.count(); i++ ) {
            if ((year === timeScaleModel.getYear(i)) && (monthNumber === timeScaleModel.getMonth(i) - 1 ))
                return i
        }
        return -1
    }
/*
    Text {
        id: searchLabel
        color: "white"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Searching in process..."
        visible: false
        z: 1000
    }
*/
/*
    Binding {
        target: timeFrameTab
        property: __isSearching
        value:  searchLabel.visible
    }
*/
    ListView {
        id: timeLine

        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width

        height: parent.height - menuBar.height - separator.height
        delegate: TimeLineDelegate {}
        model: localDayModel
        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange

        preferredHighlightBegin : timeLine.width /3
        preferredHighlightEnd : timeLine.width*2/3
        boundsBehavior : Flickable.StopAtBounds
        cacheBuffer: desktopWidth

        onCurrentIndexChanged: {
            var date = localDayModel.getDateOfIndex(timeLine.currentIndex)
            timeFrameTab.__year = date.getFullYear()
            timeFrameTab.__month = date.getMonth()
            timeFrameTab.day = date.getDay()
        }

        onCountChanged:
        {
            if(timeFrameTab.state == "") {
                timeFrameTab.state = "timeLineSearch"
                searchTimer.restart()
                timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Center)
            }
        }

    }
    Timer {
        id: searchTimer
        interval: 1000;
        onTriggered: {
            console.log("state changed")
//            if(timeFrameTab.state == "timeLineSearch") {
                timeLine.currentIndex = timeFrameTab.getTimeLineGalleryIndex()
                timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Center )
                timeFrameTab.state = ""
//            }
        }
    }


    TimeScale{
        id: timeScale
        anchors.verticalCenter: timeLine.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: 80
        width: parent.width - 100
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
            anchors.left: parent.left
        }

        Image {
            id: buttonImage
            anchors.centerIn: parent
            source: "images/gallery_normal.png"
        }

        MouseArea{
            id: stateTestButtonMouseArea
            anchors.fill: parent
            onClicked: {
                if ( timeFrameTab.state === "" ) {
                    timeFrameTab.state = "gallery"
                    galleryView.currentIndex = timeLine.currentIndex
                    galleryView.positionViewAtIndex(galleryView.currentIndex, ListView.Center )
                }
                else {
                    var index = galleryView.indexAt(galleryView.x + galleryView.width/2 + galleryView.contentX,
                                                    galleryView.y + galleryView.height/2 + galleryView.contentY)
                    timeFrameTab.state = ""
                    timeLine.currentIndex = index
                    timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Contain)

                }
            }
        }
    }
    Component {
        id: highlight
        Rectangle {
            width: galleryView.currentItem.width;
            height: galleryView.currentItem.height
            color: "#c8b0c4de"; radius: 5
            x: (photosGridView.currentIndex !== -1) ? photosGridView.currentItem.x : 0
            y: (photosGridView.currentIndex !== -1) ? photosGridView.currentItem.y : 0
            z: 0
            Behavior on x { SpringAnimation { spring: 1; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 1; damping: 0.2 } }
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
        model: localDayModel
        delegate: GalleryDelegate { }
        orientation: ListView.Horizontal
    }


    /*Timer starts wnen user start dragging gallery or timeline*/
    Timer {
        id: flickableTimer
        interval: 100; running: false; repeat: true
        onTriggered: {
            var index
            if ((timeFrameTab.state === "") || (timeFrameTab.state === "timeLineSearch"))
            {
                index = timeLine.indexAt(timeLine.x + timeLine.width/2 + timeLine.contentX,
                                         timeLine.y + timeLine.height/2 + timeLine.contentY)
            } else
            {
                index = galleryView.indexAt(galleryView.x + galleryView.width/2 + galleryView.contentX,
                                            galleryView.y + galleryView.height/2 + galleryView.contentY)
            }

            var date = localDayModel.getDateOfIndex(index)
            timeScale.list.currentIndex = getTSIndex(date.getFullYear(), date.getMonth())
            __year = date.getFullYear()
            __month = date.getMonth()
            day = date.getDay()
        }
    }


    ListView {
            id: socialView
            model: socialDayModel
            anchors.top: separator.bottom
            anchors.bottom: timeScale.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 30
            visible: false
            spacing: 10
            orientation: ListView.Horizontal

            delegate: SocialDelegate {}
        }

        SocialAuthorization {
            id: authorizationView
            visible: false
            orientation: ListView.Horizontal
            model: pluginModel
            anchors.top: separator.bottom
            anchors.bottom: timeScale.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 80
            anchors.rightMargin: 20
            anchors.topMargin: width / 8
        }

        state: socialProxy.anyPluginsEnabled() ? "socialgallery" : "socialauthorization"


    AnimatedImage {
        id: waitIndicator
        source: "images/ajax-loader.gif"
        anchors.centerIn: parent
        visible: false
    }


    Connections {
        target: galleryView
        onFlickStarted : flickableTimer.start()
    }
    Connections {
        target: timeLine
        onFlickStarted : flickableTimer.start()
    }

    Connections {
        target: galleryView
        onFlickEnded: flickableTimer.stop()
    }

    Connections {
        target: timeLine
        onFlickEnded: flickableTimer.stop()
    }

    states: [

        State {
            name: "gallery"

            AnchorChanges {
                target: timeScale
                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
            }

            PropertyChanges {
                target: timeLine
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

            PropertyChanges {
                target: stateChangeButtonText
                text : "<--"
            }
            PropertyChanges {
                target: buttonImage
                visible : false
            }
        },
        State {
            name: "gallerySearch"; extend: "gallery"
            PropertyChanges {
                target: waitIndicator
                visible: true
            }
            PropertyChanges {
                target: galleryView
                opacity: 0
            }
        },
        State {
            name: "timeLineSearch"; extend: ""
            PropertyChanges {
                target: waitIndicator
                visible: true
            }
            PropertyChanges {
                target: timeLine
                opacity: 0
            }
        },
        State {
            name: "socialgallery"

            PropertyChanges {
                target: galleryView
                visible: false
            }
            PropertyChanges {
                target: timeLine
                visible: false
            }
            PropertyChanges {
                target: socialView
                visible: true
            }

            PropertyChanges {
                target: timeScale
                opacity: 0
            }
            PropertyChanges {
                target: socialNetworks
                state: "current"
            }
            PropertyChanges {
                target: localDocs
                state: ""
            }
            PropertyChanges {
                target: socialView
                opacity: 1
            }
        },
        State {
            name: "socialauthorization"; extend: "socialgallery"

            PropertyChanges {
                target: socialView
                visible: false
            }
            PropertyChanges {
                target: socialView
                opacity: 0
            }
            PropertyChanges {
                target: authorizationView
                visible: true
            }
        }
    ]
}
