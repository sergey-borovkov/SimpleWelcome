import QtQuick 1.1
import Private 0.1
import ".."

Item {
    id: timeFrameTab
    clip: true
    property variant date: new Date()
    property bool isLocalSearching: true              //New search in process
    property bool isSocialSearching: true
    property bool direction: false  //true is - right direction; false - is left
    property bool inGallery: state === "socialGallery" || state === "gallery"
    property bool isSocial: state === "social" || state === "socialGallery"
    property bool isNepomukWorking: true
    property variant currentView: undefined
    property bool enableWheel: true
    property bool isReset: true

    Component.onCompleted: {
        mainWindow.windowHidden.connect(resetModels)
    }

    function resetModels() {
        timeFrameTab.state = ""
        localDayModel.resetModel()
        socialDayModel.resetModel()
        timeScaleModel.resetModel()
        isReset = true
    }

    function checkNepomuk()
    {
        return nepomukSource.isNepomukInitialized()
    }

    function prevMonth() {
        timeLine.decrementCurrentIndex()
    }

    function nextMonth() {
        timeLine.incrementCurrentIndex()
    }

    function getMenuItemText(id)
    {
        var txt = id
        if (id === "All")
            txt = i18n_All
        if (id === "Images")
            txt = i18n_Photo
        if (id === "Video")
            txt = i18n_Video
        if (id === "Documents")
            txt = i18n_Documents
        if (id === "Manage Networks")
            txt = i18n_Manage_networks

        var count = socialProxy.authorizedPluginCount()
        for(var i = 0; i < count; i++) {
            if (id === socialProxy.authorizedPluginName(i)) {
                txt = socialProxy.authorizedLocalizedPluginName(i)
                break;
            }
        }

        return txt
    }

    function getMenuFilterText(str)
    {
        var txt = str
        if (str === i18n_All)
            txt = "ALL"
        if (str === i18n_Photo)
            txt = "Images"
        if (str === i18n_Video)
            txt = "Video"
        if (str === i18n_Documents)
            txt = "Documents"
        if (str === i18n_Manage_networks)
            txt = "Manage Networks"

        var count = socialProxy.authorizedPluginCount()
        for(var i = 0; i < count; i++) {
            if (str === socialProxy.authorizedLocalizedPluginName(i)) {
                txt = socialProxy.authorizedPluginName(i)
                break;
            }
        }

        return txt
    }

    function closeSocialCloudItem()
    {
        for (var i = 0; i < timeFrameTab.children.length; i++) {
            if (timeFrameTab.children[i].objectName === "SocialCloudItem" ||
                    timeFrameTab.children[i].objectName === "SocialGalleryItem" ) {
                timeFrameTab.children[i].mainParent.state = ""
            }
        }
    }

    function initTimeFrame()
    {
        timeFrameTab.state = ""
        // check nepomuk
        isNepomukWorking = checkNepomuk()

        localFilterBox.state = "current"
        localFilterBox.view.currentIndex = 0
        socialFilterBox.view.currentIndex = 0

        if (isReset) {
            if (!isNepomukWorking) {
                timeFrameTab.state = "nepomukNotInit"
            } else {
                activityProxy.startSearch()
                isLocalSearching = true
                //tabListView.interactive = false
                timeFrameTab.state = "timeLineSearch"
            }
            socialProxy.startSearch()
            isReset = false
        } else {
            if (!isNepomukWorking) {
                timeFrameTab.state = "nepomukNotInit"
            } else {
                timeFrameTab.state = "timeline"
                //Set views on current date
                timeScale.list.currentIndex = timeScale.list.count -1
                timeLine.currentIndex = timeLine.count -1
                timeLine.positionViewAtEnd()
                galleryView.positionViewAtEnd()
            }
        }
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

    //Save date of current item in property "date"
    function saveCurrentDate() {
        if (!isSocial) {
            if (typeof currentView !== "undefined")
                timeFrameTab.date = localDayModel.getDateOfIndex(currentView.currentIndex)
//            else
//                timeFrameTab.date = new Date()
        }
        else
            timeFrameTab.date = socialDayModel.getDateOfIndex(currentView.currentIndex)
    }

    //Force view to repaint after applying filter
    function resetViews()  {
        var index
        if (!isSocial)
            index = localDayModel.getIndexByDate(timeFrameTab.date)
        else
            index = socialDayModel.getIndexByDate(timeFrameTab.date)

        currentView.model = undefined
        if (isSocial)
            currentView.model = socialDayModel
        else
            currentView.model = localDayModel
        currentView.currentIndex = index
        currentView.positionViewAtIndex(index, ListView.Center)
        saveCurrentDate()
        //set timeScale properly index
        timeScale.list.currentIndex = getTSIndex(timeFrameTab.date.getFullYear(), timeFrameTab.date.getMonth())
    }


    //Start initial search
    Connections {
        target:tabListView
        onCurrentIndexChanged:{
            if (tabListView.currentIndex === 3) {
                initTimeFrame()
            }
            else {
                // if need close cloud social item
                closeSocialCloudItem()
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
            if (timeFrameTab.state === "socialSearching") {
                timeFrameTab.state = "social"
                timeScale.list.currentIndex = timeScale.list.count -1
                socialTimeLine.currentIndex = socialTimeLine.count -1
                socialTimeLine.positionViewAtEnd()
            }
        }
    }

    //On local search finished
    Connections {
        target: activityProxy
        onSearchFinished: {
            isLocalSearching = false
            if (timeFrameTab.state === "timeLineSearch") {
                timeFrameTab.state = "timeline"
                //Set views on current date
                timeScale.list.currentIndex = timeScale.list.count -1
                timeLine.currentIndex = timeLine.count -1
                timeLine.positionViewAtEnd()
            }
        }
    }

    Row {
        id: menuBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: 16
        height: 30
        spacing: 10
        z: 100

        ListModel {
            id: menuDocItems
            ListElement{itemText: "All"}
            ListElement{itemText: "Images"}
            ListElement{itemText: "Video"}
            ListElement{itemText: "Documents"}
        }

        DropFilterBox{
            id: localFilterBox
            model: menuDocItems
            name: i18n_My_Local_Documents
            state: "current"

            onStateChanged: {
                if ((localFilterBox.state === "current") && ((socialFilterBox.state === "current") || (socialFilterBox.state === "open"))) {
                    socialFilterBox.state = ""
                    setLocalState()
                    setLocalFilter()

                    timeLine.currentIndex = timeLine.count - 1
                    timeLine.positionViewAtEnd()
                    timeScale.list.currentIndex = timeScale.list.count - 1
                    timeScale.list.positionViewAtIndex(timeScale.list.currentIndex, ListView.Center)
                }
            }

            onCurrentIndexChanged: {
                setLocalFilter()
                if (timeFrameTab.state !== "timeLineSearch")
                    resetViews()
            }

            function setLocalState() {
                if (!isNepomukWorking)
                    timeFrameTab.state = "nepomukNotInit"
                else if (isLocalSearching)
                    timeFrameTab.state = "timeLineSearch"
                else if (inGallery) {
                    timeFrameTab.state = "gallery"
                    updateTimeScale()
                }
                else {
                    timeFrameTab.state = "timeline"
                    updateTimeScale()
                }
            }

            function setLocalFilter() {
                saveCurrentDate()
                if(view.currentIndex === 0) {             //selectedText = "All"
                    timeScaleModel.setFilter("Local")
                    localDayModel.setFilter("Local")
                }
                else {
                    localDayModel.setFilter(getMenuFilterText(selectedText))
                    timeScaleModel.setFilter(getMenuFilterText(selectedText))
                }
            }
        }


        ListModel {
            id: menuSocialItems
            Component.onCompleted: {
                fillModel()
            }

            function fillModel() {
                clear()
                var count = socialProxy.authorizedPluginCount()
                if (count > 0)
                    append({ "itemText": "All"})

                for(var i = 0; i < count; i++)
                    append({ "itemText": socialProxy.authorizedPluginName(i) })
                append({ "itemText": "Manage Networks"})
            }
        }

        DropFilterBox{
            id: socialFilterBox
            model: menuSocialItems
            name: i18n_Social_networkong_sites
            onStateChanged: {
                if ((socialFilterBox.state === "current") && ((localFilterBox.state === "current") || (localFilterBox.state === "open"))) {
                    if (socialFilterBox.view.count < 2) { //check accounts count: if no ones is loggin in show SocialAuthorization page
                        timeFrameTab.state = "socialAuthorization"
                        socialFilterBox.view.currentIndex = 0
                    } else
                    {
                        setSocialState()
                    }
                    localFilterBox.state = ""
                    setSocialFilter()

                    socialTimeLine.currentIndex = socialTimeLine.count - 1
                    socialTimeLine.positionViewAtEnd()

                    timeScale.list.currentIndex = timeScale.list.count - 1
                    timeScale.list.positionViewAtIndex(timeScale.list.currentIndex, ListView.Center)
                }
            }
            onCurrentIndexChanged: {
                if ((timeFrameTab.state !== "socialAuthorization") && (timeFrameTab.state !== "socialSearching"))
                    saveCurrentDate()
                setSocialState()
                setSocialFilter()
                if ((timeFrameTab.state !== "socialAuthorization") && (timeFrameTab.state !== "socialSearching"))
                    resetViews()
            }

            function setSocialState() {
                if (socialFilterBox.view.currentIndex === socialFilterBox.view.count -1)
                    timeFrameTab.state = "socialAuthorization"
                else if (isSocialSearching)
                    timeFrameTab.state = "socialSearching"
                else if (inGallery)
                    timeFrameTab.state = "socialGallery"
                else
                    timeFrameTab.state = "social"
            }

            function setSocialFilter() {
                if(view.currentIndex === 0) { //selectedText = "All"
                    timeScaleModel.setFilter("Social")
                    socialDayModel.setFilter("Social")
                }
                else {
                    timeScaleModel.setFilter(getMenuFilterText(selectedText))
                    socialDayModel.setFilter(getMenuFilterText(selectedText))
                }
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
        visible: false
    }

    Item {
        id: warningButton
        visible: false
        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        Item {
            anchors.centerIn: parent
            Text {
                id: messageInfo
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: warningTabButton.top
                anchors.margins: 20
                text: i18n_Nepomuk_disabled
                color: "white"
                font.pointSize: 12
            }

            Item {
                id: warningTabButtonBg

                width: (parent.width-4) / 3
                height: 27
                visible: tabListView.currentIndex != 0
                anchors.fill: warningTabButton
                //z: -1

                property alias pressedAndHovered: warningTabButton.pressedAndHovered

                BorderImage {
                    border.left: 6
                    border.right: 7
                    border.top: 0
                    border.bottom: 0
                    anchors.fill: parent
                    anchors.rightMargin: -3
                    source: warningTabButtonBg.pressedAndHovered ? "image://generalicon/asset/tab_button_pressed.png" : "image://generalicon/asset/tab_button.png"
                }

                Behavior on x {
                    NumberAnimation { duration: 200; }
                }
            }

            TabButton {
                id: warningTabButton
                anchors.centerIn: parent
                label: i18n_Enable
                pressable: true
                active: true

                onButtonClick: {
                    nepomukSource.nepomukConfigure()
                    mainWindow.close()
                }
                //buttonWidth: warningButton.width / 8
            }
        }
    }

    ListView {
        id: timeLine
        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        visible: false
        delegate: TimeLineDelegate {}
        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange

        preferredHighlightBegin : timeLine.width /3
        preferredHighlightEnd : timeLine.width*2/3
        boundsBehavior : Flickable.StopAtBounds
        cacheBuffer: desktopWidth
        opacity: 0

        WheelArea {
            id: timeLineWheelArea
            anchors.fill: parent
            onScrollVert: _processScroll(delta, timeLine)
        }
        Rectangle{
            anchors.fill: parent
            color: "transparent"
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

        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin : timeLine.width /3
        preferredHighlightEnd : timeLine.width*2/3
        boundsBehavior : Flickable.StopAtBounds
        visible: false
        cacheBuffer: desktopWidth
        opacity: 0

        WheelArea {
            id: socialTimeLineWheelArea
            anchors.fill: parent
            onScrollVert: _processScroll(delta, socialTimeLine)
            enabled: enableWheel
        }
        Rectangle{
            anchors.fill: parent
            color: "transparent"
        }
    }

    TimeScale {
        id: timeScale
        anchors.verticalCenter: timeLine.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: 80
        width: parent.width - 100

        WheelArea {
            id: timeScaleWheelArea
            anchors.fill: parent
            onScrollVert: _processScroll(delta, timeScale.list)
            enabled: enableWheel
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
                var index
                if (inGallery)
                {
                    index = currentView.indexAt(currentView.x + currentView.width/2 + currentView.contentX,
                                                currentView.y + currentView.height/2 + currentView.contentY)
                    if (isSocial)
                        timeFrameTab.state = "social"
                    else
                        timeFrameTab.state = "timeline"
                    currentView.currentIndex = index
                    currentView.positionViewAtIndex(currentView.currentIndex, ListView.Contain)
                } else {
                    index = currentView.currentIndex
                    if (isSocial)
                        timeFrameTab.state = "socialGallery"
                    else
                        timeFrameTab.state = "gallery"
                    currentView.currentIndex = index
                    currentView.positionViewAtIndex(currentView.currentIndex, ListView.Center)
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

        delegate: SocialGalleryDelegate {}

        visible: false
        spacing: 10
        orientation: ListView.Horizontal
        boundsBehavior : Flickable.StopAtBounds

        WheelArea {
            id: socialGalleryViewWhellArea
            anchors.fill: parent
            onScrollVert: _processScroll(delta, socialGalleryView)
            enabled: enableWheel
        }
    }

    /*Timer starts wnen user starts dragging gallery or timeline*/
    Timer {
        id: flickableTimer
        interval: 100; running: false; repeat: true
        onTriggered: updateTimeScale()
    }

    function updateTimeScale() {
        if (currentView === undefined)
            return

        var index = 0
        var date = new Date()
        index = currentView.indexAt(currentView.x + currentView.width/2 + currentView.contentX,
                                    currentView.y + currentView.height/2 + currentView.contentY)

        date = isSocial ? socialDayModel.getDateOfIndex(index) : localDayModel.getDateOfIndex(index)

        if (index === -1)
            return

        timeScale.list.currentIndex = getTSIndex(date.getFullYear(), date.getMonth())
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

    Image {
        id: waitIndicator
        anchors.centerIn: parent        
        source: "images/indicator-shadow.png"
        opacity: 0
        Image {
            id: can
            source: "images/can2.png"
            smooth: true
        }
        Behavior on opacity { NumberAnimation{ duration: 1000 }}
    }

    Timer {
        id: waitTimer
        interval: 10; running: false; repeat: true
        onTriggered: can.rotation = can.rotation +10
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
        // we have to pass point over list item otherwise indexAt() will return -1, so, we add small offset
        var index = listView.indexAt(listView.contentX + 3, listView.contentY + 3);
        if (index < 0)
            return
        index -= index_delta;
        if (index < 0)
            listView.positionViewAtBeginning()
        else if (index >= listView.count)
            listView.positionViewAtEnd()
        else
            listView.positionViewAtIndex(index, ListView.Beginning)
        updateTimeScale()
    }

    // States
    state: ""
    states: [

        State {
            name: "nepomukNotInit";

            PropertyChanges { target: warningButton; visible: true }

            PropertyChanges { target: timeScale; visible: false; opacity: 0 }

            PropertyChanges { target: galleryButton; visible: false }
        },
        State {
            name: "timeline"
            PropertyChanges { target: timeLine;  visible : true; model: localDayModel; opacity: 1 }

            PropertyChanges { target: timeFrameTab;  currentView: timeLine }
        },
        State {
            name: "gallery"

            AnchorChanges {
                target: timeScale
                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
            }
            PropertyChanges { target: timeScale; anchors.bottomMargin: 20 }

            PropertyChanges { target: galleryView; visible: true }

            PropertyChanges { target: galleryView; model: localDayModel }

            PropertyChanges { target: timeFrameTab; currentView: galleryView }

        },
        State {
            name: "timeLineSearch"

            PropertyChanges { target: waitIndicator; opacity: 1 }

            PropertyChanges { target: waitTimer; running: true }

            PropertyChanges { target: timeScale; visible: false; opacity: 0 }

            PropertyChanges { target: galleryButton; visible: false; opacity: 0 }
        },
        State {
            name: "social"

            PropertyChanges { target: socialTimeLine; visible: true; opacity: 1; model: socialDayModel }

            PropertyChanges { target: timeFrameTab; currentView: socialTimeLine }

        },
        State {
            name: "socialGallery"; extend: "social"

            PropertyChanges { target: socialTimeLine; visible: false; opacity: 0 }

            PropertyChanges { target: socialGalleryView; visible: true; model: socialDayModel }

            AnchorChanges {
                target: timeScale
                anchors.verticalCenter: undefined
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom:  timeFrameTab.bottom
            }
            PropertyChanges { target: timeScale; anchors.bottomMargin: 20 }

            PropertyChanges { target: socialGalleryView; opacity: 1 }

            PropertyChanges { target: timeFrameTab; currentView: socialGalleryView }
        },
        State {
            name: "socialAuthorization"; extend: "social"

            PropertyChanges { target: socialTimeLine; visible: false; opacity: 0 }

            PropertyChanges { target: authorizationView; visible: true }

            PropertyChanges { target: timeScale; visible: false; opacity: 0 }

            PropertyChanges { target: galleryButton; visible: false; opacity: 0 }
        },
        State {
            name: "socialSearching"; extend: "social"

            PropertyChanges { target: socialTimeLine; visible: false; opacity: 0; }

            PropertyChanges { target: waitIndicator; opacity: 1 }

            PropertyChanges { target: waitTimer; running: true }

            PropertyChanges { target: galleryButton; visible: false }

            PropertyChanges { target: timeScale; visible: false; opacity: 0 }
        }
    ]
}
