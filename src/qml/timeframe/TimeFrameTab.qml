import QtQuick 1.1
import Private 0.1

Item {
    id: timeFrameTab
    clip: true

    property int __year: new Date().getFullYear()   //Current year
    property int __month: new Date().getMonth()
    property int day: new Date().getDate()
    property bool __isLocalSearching: true              //New search in process
    property bool isSocialSearching: true
    property bool direction: false  //true is - right direction; false - is left
    property bool inGallery: state === "socialGallery" || state === "gallery" || state === "gallerySearch"

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

    function getMenuItemText(id)
    {
        var txt = id
        if (id == "All")
            txt =  i18n_All
        if (id == "Photo")
            txt =  i18n_Photo
        if (id == "Video")
            txt =  i18n_Video
        if (id == "Documents")
            txt =  i18n_Documents
        if (id == "Manage networks")
            txt =  i18n_Manage_networks
        if (id == "Documents")
            txt =  i18n_Documents
        return txt
    }

    //Start initial search
    Connections {
        target:tabListView
        onCurrentIndexChanged:{
            if (tabListView.currentIndex === 3) {
                activityProxy.startSearch()
                tabListView.interactive = false
                if ((__isLocalSearching) && (timeFrameTab.state ==="")) {
                    // to prevent items created and destroyed since
                    // listview still exists and creates components
                    timeLine.model = undefined
                    timeFrameTab.state = "timeLineSearch"
                }
            }
        }
    }

    Connections {
        target: socialProxy
        onSearchStarted: {
            isSocialSearching = true
            if (timeFrameTab.state === "social") {
                timeFrameTab.state = "socialSearching"
            }
        }
    }

    Connections {
        target: socialProxy
        onSearchFinished: {
            isSocialSearching = false
            if (timeFrameTab.state === "socialSerching") {
                timeFrameTab.state = "social"
            }
        }
    }

    //On local search finished
    Connections {
        target: activityProxy
        onFinished: {
            __isLocalSearching = false
            if (timeFrameTab.state === "timeLineSearch") {
                //Set views on current date
                timeScale.list.currentIndex = timeScale.list.count -1
                timeLine.model = localDayModel
                timeLine.currentIndex = timeLine.count -1
                timeLine.positionViewAtEnd()
                galleryView.positionViewAtEnd()

                timeFrameTab.state = ""
            }
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
            ListElement{itemText: "All"}
            ListElement{itemText: "Photo"}
            ListElement{itemText: "Video"}
            ListElement{itemText: "Documents"}
        }

        ListModel {
            id: menuSocialItems
            Component.onCompleted: {
                fillModel()
            }

            function fillModel() {
                clear()
                var count = socialProxy.authorizedPluginCount()
                append({ "itemText": "All"})

                for(var i = 0; i < count; i++)
                    append({ "itemText": socialProxy.authorizedPluginName(i) })
                append({ "itemText": "Manage networks"})
            }
        }

        DropListBox {
            id: localDocs
            width: 330
            model: menuDocItems
            name: i18n_My_Local_Documents
            selectedText: name
            state: "current"

            function setLocalFilter()
            {
                if (name === i18n_My_Local_Documents) {
                    timeScaleModel.setFilter("Local")
                    localDayModel.setFilter("Local")
                }
                else {
                    timeScaleModel.setFilter(selectedText)
                    localDayModel.setFilter(selectedText)
                }

            }
            onSelectedIndexChanged: {
                name = ( selectedText === "All" ) ? i18n_My_Local_Documents : getMenuItemText(selectedText)
                setLocalFilter()
            }
            onClicked: {
                if ((timeFrameTab.state != "") && (timeFrameTab.state != "gallery")) {
                    timeFrameTab.state = inGallery ? "gallery" : ""
                    setLocalFilter()
                }
            }
        }

        DropListBox {
            id: socialNetworks
            width: 260
            model: menuSocialItems
            name: i18n_Social_networkong_sites
            selectedText: name
            property int __wasSearching: 0

            function setSocialFilter()
            {
                if (name === i18n_Social_networkong_sites) {
                    timeScaleModel.setFilter("Social")
                    socialDayModel.setFilter("Social")
                }
                else {
                    socialDayModel.setFilter(selectedText)
                    timeScaleModel.setFilter(selectedText)
                }
            }

            onSelectedIndexChanged: {
                if (selectedText === "Manage networks")
                    timeFrameTab.state = "socialAuthorization"
                else if (isSocialSearching)
                    timeFrameTab.state = "socialSerching"
                else if (inGallery)
                    timeFrameTab.state = "socialGallery"
                else
                    timeFrameTab.state = "social"

                name = ( selectedText === "All" ) ? i18n_Social_networkong_sites : getMenuItemText(selectedText)
                setSocialFilter()
            }
            onClicked: {
                if (timeFrameTab.state != "socialAuthorization" && socialProxy.anyPluginsEnabled()) {
                    if (isSocialSearching)
                        timeFrameTab.state = "socialSerching"
                    else
                        timeFrameTab.state =  inGallery ? "socialGallery" : "social"
                    setSocialFilter()
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
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        delegate: TimeLineDelegate {}
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

        WheelArea {
            anchors.fill: parent
            onScrollVert: _processScroll(delta, timeLine)
            onScrollHorz: _processScroll(delta, timeLine)
        }
    }

    ListView {
        id: socialTimeLine

        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.topMargin: 10

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

        WheelArea {
            anchors.fill: parent
            onScrollVert: _processScroll(delta, socialTimeLine)
            onScrollHorz: _processScroll(delta, socialTimeLine)
        }
    }

    TimeScale {
        id: timeScale
        anchors.verticalCenter: timeLine.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: 80
        width: parent.width - 100

        WheelArea {
            anchors.fill: parent
            onScrollVert: _processScroll(delta, timeScale.list)
            onScrollHorz: _processScroll(delta, timeScale.list)
        }
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

    Item {
        id: galleryButton
        anchors.bottom: separator.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 100
        height: 32
        Image {
            id: galleryButtonImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            source: "images/gallery.png"
        }
        MouseArea {
            id: galleryButtonMouseArea
            anchors.fill: parent
            hoverEnabled: true
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
                else {
                    timeFrameTab.state = "social"
                }
            }
        }
        states: [
            State {
                name: "gallery"; when: ((timeFrameTab.state === "socialGallery") || (timeFrameTab.state === "gallery"))
                PropertyChanges {  target: galleryButtonImage;  source: "images/back.png" }
            }
        ]

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
        delegate: GalleryDelegate { }
        orientation: ListView.Horizontal
        boundsBehavior : Flickable.StopAtBounds

        WheelArea {
            anchors.fill: parent
            onScrollVert: _processScroll(delta, galleryView)
            onScrollHorz: _processScroll(delta, galleryView)
        }
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

        WheelArea {
            anchors.fill: parent
            onScrollVert: _processScroll(delta, socialGalleryView)
            onScrollHorz: _processScroll(delta, socialGalleryView)
        }
    }

    /*Timer starts wnen user starts dragging gallery or timeline*/
    Timer {
        id: flickableTimer
        interval: 100; running: false; repeat: true
        onTriggered: updateTimeScale()

        function updateTimeScale() {
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
            if (index === -1)
                return

            timeScale.list.currentIndex = getTSIndex(date.getFullYear(), date.getMonth())

            __year = date.getFullYear()
            __month = date.getMonth()
            day = date.getDay()
        }
    }

    SocialAuthorization {
        id: authorizationView
        visible: false
        model: pluginModel
        anchors.top: separator.bottom
        anchors.bottom: parent.bottom

        //Rectangle to force repaint
        Rectangle {
            anchors.fill: parent
            color: "transparent"
        }
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

    Item {
        id: foreground
        anchors.fill: parent
    }

    // Functions
    /**
     * Processes scroll using mouse wheel
     * @param delta A delta
     * @param listView An instance of ListView which will be updated
    */
    function _processScroll(delta, listView) {
        // See Qt documentation of QGraphicsSceneWheelEvent
        // Most mice report delta = 120
        var index_delta = Math.round(delta / 120)
        if (index_delta === 0)
            index_delta = (delta > 0 ? 1 : -1)
        var index = listView.indexAt(listView.contentX, listView.contentY);
        if (index < 0)
            return
        index -= index_delta;
        if (index < 0)
            listView.positionViewAtBeginning()
        else if (index >= listView.count)
            listView.positionViewAtEnd()
        else
            listView.positionViewAtIndex(index, ListView.Beginning)
        flickableTimer.updateTimeScale()
    }

    // States
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

            PropertyChanges { target: timeLine;  visible: false }

            PropertyChanges { target: galleryView; visible: true }

            PropertyChanges { target: galleryView; model: localDayModel }
            //PropertyChanges { target: galleryView; enabled: true }
        },
        State {
            name: "gallerySearch"; extend: "gallery"

            PropertyChanges { target: waitIndicator; visible: true }

            PropertyChanges { target: galleryView; opacity: 0 }
        },
        State {
            name: "timeLineSearch"; extend: ""

            PropertyChanges { target: waitIndicator; visible: true }

            PropertyChanges { target: timeScale; visible: false }

            PropertyChanges { target: timeLine; opacity: 0 }

            PropertyChanges {
                target: galleryButton
                visible: false
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

            PropertyChanges { target: socialTimeLine; visible: true }

            PropertyChanges { target: socialNetworks; state: "current" }

            PropertyChanges { target: localDocs; state: "" }
        },

        State {
            name: "socialGallery"; extend: "social"

            PropertyChanges {
                target: socialTimeLine
                visible: false
                opacity:0
            }

            PropertyChanges { target: socialGalleryView; visible: true }

            AnchorChanges {
                target: timeScale
                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
            }

            PropertyChanges { target: socialGalleryView; opacity: 1 }
        },
        State {
            name: "socialAuthorization"; extend: "social"

            PropertyChanges {
                target: socialTimeLine
                visible: false
                opacity: 0
            }
            PropertyChanges { target: authorizationView; visible: true }

            PropertyChanges {
                target: timeScale
                visible: false
                opacity: 0
            }
            PropertyChanges {
                target: galleryButton
                visible: false
                opacity: 0
            }
        },
        State {
            name: "socialSerching"; extend: "social"

            PropertyChanges {
                target: socialTimeLine
                visible: false
                opacity: 0
            }

            PropertyChanges { target: waitIndicator; visible: true }

            PropertyChanges { target: galleryButton; visible: false }
        }
    ]
}
