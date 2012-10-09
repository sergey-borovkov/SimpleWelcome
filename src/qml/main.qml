import QtQuick 1.1

import Qt 4.7
import "timeframe"

Item {
//Rectangle {
    id: page
    width: mainWindow.updatableWidth
    height: mainWindow.updatableHeight
    //color: "black"

    property bool isCompleted: false

    function onWindowSizeChanged(inWidth, inHeight) {
        if (isCompleted)
        {
            console.log("resize to " + width + "x" + height)
            searchTab.tab.updateGridsContent()
            welcomeTab.tab.updateGridsContent()
            appsTab.tab.updateGridsContent() // FIX LATER
        }
    }

    Keys.onPressed: {
        if(event.key == Qt.Key_Escape) {
            event.accepted = true;
            mainWindow.close();
        }
        else if (event.key == Qt.Key_Space) {
            event.accepted = true
            tabListView.currentTabIndexChanged((tabListView.currentIndex + tabListView.count + 1) % tabListView.count + 1)
        }
    }

    Component.onCompleted: {
        isCompleted = true
        console.log("completed with: " + width + "x" + height + "")
        mainWindow.windowHid.connect(onWindowHid)
        mainWindow.windowSizeChanged.connect(onWindowSizeChanged)
        bottomBar.wheelScroll.connect(tabListView.onWheelScroll)
    }

    function onWindowHid() {
        topBar.searchText = ""
        tabListView.currentIndex = 1
        topBar.forceActiveFocus()
    }

    NumberAnimation on opacity { to: 1.0; duration: 500 }

    /*Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: "image://generalicon/asset/bubbles.jpg"
        z: -10
    }*/

    Rectangle {
        anchors.fill: parent
//        color: "black"
//        opacity: 0.7
        color: "#263C50"
        opacity: 0.9

    }
    VisualItemModel {
        id: tabListModel

        SearchTab {
            id: searchTab
            width: tabListView.width
            height: tabListView.height
        }

        WelcomeTab {
            id: welcomeTab
            width: tabListView.width
            height: tabListView.height
        }

        ApplicationsTab {
            id: appsTab
            width: tabListView.width
            height: tabListView.height
        }

        /*Text {
            text: "TimeFrame is disabled\nin this internal pre-alpha."
            width: tabListView.width
            height: tabListView.height
            color: "gray"
            font.pointSize: 30
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideMiddle

        }*/
        TimeFrameTab {
            width: tabListView.width
            height: tabListView.height
        }
    }

    TopBar {
        id: topBar
    }

    /*Rectangle{
        color: Qt.rgba(0, 0, 0, 0.3)
        anchors.fill: tabListView
    }*/

    ListView {
        id: tabListView

        KeyNavigation.tab: topBar
        KeyNavigation.backtab: topBar

        interactive: false

        anchors.top: topBar.bottom
        width: parent.width
        height: parent.height - topBar.height - bottomBar.height

        model: tabListModel

        snapMode: ListView.SnapOneItem
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickDeceleration: 5000
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 240
        highlightRangeMode: ListView.StrictlyEnforceRange

        currentIndex: 1

        function processKeyboard(key) {
            if (currentItem)
                currentItem.tab.processKeyboard(key)

        }

        onCurrentIndexChanged: {
            if (currentItem && currentItem.grid)
                currentItem.grid.forceMyFocus()
            topBar.forceActiveFocus()
        }

        function currentTabIndexChanged(newCurrentIndex) {
            tabListView.currentIndex = newCurrentIndex
        }

        function onWheelScroll(delta) {
            // See Qt documentation of QGraphicsSceneWheelEvent
            // Most mice report delta = 120
            var pages_delta = Math.round(delta / 120)
            if (pages_delta === 0)
                pages_delta = (delta > 0 ? 1 : -1)

            var count2 = count - 1
            currentTabIndexChanged((currentIndex + count2 - pages_delta - 1) % count2 + 1)
        }

        Component.onCompleted: {
            searchGridModel.currentTabIndexChanged.connect(currentTabIndexChanged)
        }
    }


    BottomBar {
        id: bottomBar
        width: parent.width
    }

}
