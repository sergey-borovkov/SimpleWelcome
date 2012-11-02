import QtQuick 1.1

import Qt 4.7
import "timeframe"

Item {
    id: rootWrapper

    width: mainWindow.updatableWidth
    height: mainWindow.updatableHeight

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

    Item {
        id: root
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        property bool isCompleted: false

        function onWindowSizeChanged() {
            if (isCompleted)
            {
                var wasCurrentIndex = tabListView.currentIndex
                var margins = mainWindow.getMargins()
                anchors.leftMargin = margins.x
                anchors.topMargin = margins.y
                anchors.rightMargin = margins.width
                anchors.bottomMargin = margins.height
                console.log("resize to " + width + "x" + height + " [" + margins.x + "," + margins.y + "," + margins.width  + "," + margins.height + "]")

                searchTab.tab.updateGridsContent()
                welcomeTab.tab.updateGridsContent()
                appsTab.tab.updateGridsContent() // FIX LATER
                tabListView.currentIndex = 0 // hack needed to remain on the same tab we was before
                tabListView.currentIndex = wasCurrentIndex
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

        /*function cloneObject(obj) {
            // Copying object by value
            var newObj = new Object
            for (var s in (obj)) {
                //console.log("copying " + itemStackingTo[s])
                newObj[s] = (obj)[s]
            }
            return newObj
        }*/


        function cloneObject(inObj) {
            var newObj = (inObj instanceof Array) ? [] : {}
            for (var i in inObj) {

                if (inObj[i] && typeof inObj[i] == "object")
                    newObj[i] = cloneObject(inObj[i])
                else
                    newObj[i] = inObj[i]
            }
            return newObj
        }


        Component.onCompleted: {
            isCompleted = true
            console.log("completed with: " + width + "x" + height + "")
            mainWindow.windowHidden.connect(onWindowHid)
            mainWindow.windowSizeChanged.connect(onWindowSizeChanged)
            bottomBar.wheelScroll.connect(tabListView.onWheelScroll)

        }

        function onWindowHid() {
            topBar.searchText = ""
            tabListView.currentIndex = 1
            topBar.forceActiveFocus()
        }

        NumberAnimation on opacity { to: 1.0; duration: 500 }

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

//        Rectangle{
//            color: Qt.rgba(0, 0, 0, 0.2)
//            anchors.fill: tabListView
//        }

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
                mainWindow.currentTabChanged(currentIndex)
            }

            function currentTabIndexChanged(newCurrentIndex) {
                tabListView.currentIndex = newCurrentIndex
            }

            function onWheelScroll(delta) {
                if (delta > 0) {
                    if (currentIndex > 1)
                        tabListView.decrementCurrentIndex()
                }
                else
                    tabListView.incrementCurrentIndex()
            }

            Component.onCompleted: {
                searchGridModel.currentTabIndexChanged.connect(currentTabIndexChanged)
            }
        }

        TopBar {
            id: topBar
        }

        BottomBar {
            id: bottomBar
            width: parent.width
        }

    }
}
