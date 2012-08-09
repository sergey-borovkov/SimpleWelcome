import QtQuick 1.1

Item {
    id: timeFrameTab
    width: parent.width
    clip: true
    anchors.topMargin: 16

    property int __year: new Date().getFullYear()   //Current year
    property int __month: new Date().getMonth()
    property int day: new Date().getDate()
    property bool __isSearching: false              //New search in process
    property bool direction: false  //true is - right direction; false - is left

    function getTimeLineIndex() {
        var index = localDayModel.getIndexByDate(__year, __month+1, direction)
        //console.log("getIndexByDate: " + index)
        return index
    }

    function getSocialTimeLineIndex() {
        var index = socialDayModel.getIndexByDate(__year, __month+1, direction)
        //console.log("social index " + index)
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
                fillModel()
            }

            function fillModel() {
                clear()
                var count = socialProxy.authorizedPluginCount()
                if(count > 0)
                    append({ "itemText": "All"})

                for(var i = 0; i < count; i++)
                    append({ "itemText": socialProxy.authorizedPluginName(i) })
                append({ "itemText": "Manage networks"})
            }
        }

        DropListBox {
            id: localDocs
            model: menuDocItems
            name: "My Local Documents"
            state: "current"
            onSelectedIndexChanged: {
                if (selectedText === "All")
                    timeScaleModel.setFilter("Local")
                else
                    timeScaleModel.setFilter(selectedText)
                localDayModel.setFilter(selectedText)
                state = "current"
                socialNetworks.state = ""
            }
            onClicked: {
                state = "current"
                timeFrameTab.state = ""
                socialNetworks.state = ""
                timeScaleModel.setFilter("Local")
            }
        }

        DropListBox {
            id: socialNetworks
            model: menuSocialItems
            name: "Social networking sites"
            property int __wasSearching: 0
            onSelectedIndexChanged: {
                if(selectedText === "Manage networks")
                    timeFrameTab.state = "socialAuthorization"
                else {
                    timeFrameTab.state = "social"
                    socialProxy.startSearch()
                    if (selectedText === "All")
                    {
                        timeScaleModel.setFilter("Social")
                        socialDayModel.setFilter("Social")
                    }
                    else
                    {
                        socialDayModel.setFilter(selectedText)
                        timeScaleModel.setFilter(selectedText)
                    }
                }
                state = "current"
                localDocs.state = ""
            }
            onClicked: {
                state = "current"
                localDocs.state = ""
                if (timeFrameTab.state != "socialAuthorization" && socialProxy.anyPluginsEnabled()) {
                    timeFrameTab.state = "social"
                    if ((selectedText === "All") || (selectedText === ""))
                        timeScaleModel.setFilter("Social")
                    else
                        timeScaleModel.setFilter(selectedText)
                }
                else
                    timeFrameTab.state = "socialAuthorization"
            }
            Connections {
                target: socialProxy
                // simply reset model if authorized plugins changed
                onPluginAuthorized: {
                    menuSocialItems.fillModel()
                }
                onPluginDeauthorized: {
                    menuSocialItems.fillModel()
                }
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

    //get index of date in TimeScale
    function getTSIndex(year, monthNumber) {
        var i
        for (i = 0; i < timeScaleModel.count(); i++ ) {
            if ((year === timeScaleModel.getYear(i)) && (monthNumber === timeScaleModel.getMonth(i) - 1 ))
                return i
        }
        return -1
    }

    ListView {
        id: timeLine

        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width


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
            if ( (timeFrameTab.state === "social") || (timeFrameTab.state === "socialGallery") )
                return
            timeLine.currentIndex = timeFrameTab.getTimeLineIndex()
            timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Center )
            timeFrameTab.state = ""
        }
    }
    ListView {
        id: socialTimeLine

        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        anchors.bottom: parent.bottom

        delegate: SocialCloudDelegate {}
        model: socialDayModel

        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin : timeLine.width /3
        preferredHighlightEnd : timeLine.width*2/3
        boundsBehavior : Flickable.StopAtBounds
        visible: false
        cacheBuffer: desktopWidth

        onCurrentIndexChanged: {
            var date = socialDayModel.getDateOfIndex(timeLine.currentIndex)
            timeFrameTab.__year = date.getFullYear()
            timeFrameTab.__month = date.getMonth()
            timeFrameTab.day = date.getDay()
        }
    }
    TimeScale {
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
                else if ( timeFrameTab.state === "gallery" ) {
                    var index = galleryView.indexAt(galleryView.x + galleryView.width/2 + galleryView.contentX,
                                                    galleryView.y + galleryView.height/2 + galleryView.contentY)
                    timeFrameTab.state = ""
                    timeLine.currentIndex = index
                    timeLine.positionViewAtIndex(timeLine.currentIndex, ListView.Contain)

                }
                else if ( timeFrameTab.state === "social" ) {
                    timeFrameTab.state = "socialGallery"
                }
                else timeFrameTab.state = "social"
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
        boundsBehavior : Flickable.StopAtBounds
    }

    ListView {
        id: socialGalleryView

        anchors.top: separator.bottom
        anchors.bottom: timeScale.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        model: socialDayModel
        delegate: SocialGalleryDelegate {}

        visible: false
        spacing: 10
        orientation: ListView.Horizontal
        boundsBehavior : Flickable.StopAtBounds

    }

    /*Timer starts wnen user start dragging gallery or timeline*/
    Timer {
        id: flickableTimer
        interval: 100; running: false; repeat: true
        onTriggered: {
            var index = 0
            var date = new Date()
            if ((timeFrameTab.state === "") || (timeFrameTab.state === "timeLineSearch"))
            {
                index = timeLine.indexAt(timeLine.x + timeLine.width/2 + timeLine.contentX,
                                         timeLine.y + timeLine.height/2 + timeLine.contentY)
                date = localDayModel.getDateOfIndex(index)
            } else if (timeFrameTab.state === "gallery")
            {
                index = galleryView.indexAt(galleryView.x + galleryView.width/2 + galleryView.contentX,
                                            galleryView.y + galleryView.height/2 + galleryView.contentY)
                date = localDayModel.getDateOfIndex(index)
            } else if (timeFrameTab.state === "social")
            {
                index = socialTimeLine.indexAt(socialTimeLine.x + socialTimeLine.width/2 + socialTimeLine.contentX,
                                            socialTimeLine.y + socialTimeLine.height/2 + socialTimeLine.contentY)
                date = socialDayModel.getDateOfIndex(index)
            } else if (timeFrameTab.state === "socialGallery")
            {
                index = socialGalleryView.indexAt(socialGalleryView.x + socialGalleryView.width/2 + socialGalleryView.contentX,
                                            socialGalleryView.y + socialGalleryView.height/2 + socialGalleryView.contentY)
                date = socialDayModel.getDateOfIndex(index)
            }


            timeScale.list.currentIndex = getTSIndex(date.getFullYear(), date.getMonth())

            __year = date.getFullYear()
            __month = date.getMonth()
            day = date.getDay()
        }
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
        target: socialTimeLine
        onFlickStarted : flickableTimer.start()
    }
    Connections {
        target: socialGalleryView
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
    Connections {
        target: socialTimeLine
        onFlickEnded: flickableTimer.stop()
    }
    Connections {
        target: socialGalleryView
        onFlickEnded: flickableTimer.stop()
    }

    state: ""
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
            name: "social"
            PropertyChanges {
                target: timeLine
                visible: false
                opacity: 0
            }
            PropertyChanges {
                target: socialTimeLine
                visible: true
            }
            PropertyChanges {
                target: socialNetworks
                state: "current"
            }
            PropertyChanges {
                target: localDocs
                state: ""
            }

        },

        State {
            name: "socialGallery"; extend: "social"

            PropertyChanges {
                target: socialTimeLine
                visible: false
                opacity:0
            }

            PropertyChanges {
                target: socialGalleryView
                visible: true
            }

            AnchorChanges {
                target: timeScale
                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
            }

            PropertyChanges {
                target: socialGalleryView
                opacity: 1
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
            name: "socialAuthorization"; extend: "social"

            PropertyChanges {
                target: socialTimeLine
                visible: false
                opacity: 0
            }
            PropertyChanges {
                target: authorizationView
                visible: true
            }
            PropertyChanges {
                target: timeScale
                visible: false
                opacity: 0
            }
        }
    ]
}
