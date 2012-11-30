import QtQuick 1.1
import "timeframe"

Item {
    id: rootWrapper

    Connections {
        target: mainWindow

        onAvailableGeometryChanged: root.updateSize()

        onWindowHidden: {
            topBar.searchText = ""
            tabListView.currentIndex = 1
            topBar.forceActiveFocus()
        }
    }

    Binding {
        target: dataSource_Apps
        property: "isUpdateAllowed"
        value: !mainWindow.visible
    }

    Binding {
        target: dataSource_Favorites
        property: "isUpdateAllowed"
        value: !mainWindow.visible
    }

    Binding {
        target: dataSource_Documents
        property: "isUpdateAllowed"
        value: !mainWindow.visible
    }


    Image {
        id: backgroundImage
        anchors.fill: parent
        z: -10

        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: "image://generalicon/asset/background.jpg"
    }

    Item {
        id: root
        anchors {
            fill: parent
            leftMargin: mainWindow.availableGeometry.x
            topMargin: mainWindow.availableGeometry.y
            rightMargin: rootWrapper.width - anchors.leftMargin - mainWindow.availableGeometry.width
            bottomMargin: rootWrapper.height - anchors.topMargin - mainWindow.availableGeometry.height
        }

        property bool isCompleted: false

        Keys.onPressed: {
            if(event.key === Qt.Key_Escape) {
                event.accepted = true;
                var currentTab = tabListView.currentItem
                if (currentTab && currentTab.tab && currentTab.tab.isPopupOpened)
                    currentTab.tab.closePopup()
                else
                    mainWindow.close();
            }
            else if (event.key === Qt.Key_Space) {
                event.accepted = true
                tabListView.currentTabIndexChanged((tabListView.currentIndex + tabListView.count + 1) % tabListView.count + 1)
            }
        }

        function updateSize() {
            if (isCompleted) {
                var wasCurrentIndex = tabListView.currentIndex
                searchTab.tab.updateGridsContent()
                welcomeTab.tab.updateGridsContent()
                appsTab.tab.updateGridsContent() // FIX LATER
                tabListView.currentIndex = 0 // hack needed to remain on the same tab we was before
                tabListView.currentIndex = wasCurrentIndex
            }
        }

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

        function initDefaultProperties(item) {
            // This is needed for delegate to not blaming unknown variables
            if (item.pinned === undefined)
                item.pinned = undefined
            if (item.stack === undefined)
                item.stack = undefined
        }

        function prependItemWithStack(model, item) {
            // This is needed to prevent ListModel.append from converting JsObject to ListModel
            var itemStack = item.stack
            item.stack = undefined
            initDefaultProperties(item)

            model.insert(0, item)

            if (itemStack !== undefined)
                model.setProperty(0, "stack", itemStack)
        }

        function appendItemWithStack(model, item) {
            // This is needed to prevent ListModel.append from converting JsObject to ListModel
            var itemStack = item.stack
            item.stack = undefined
            initDefaultProperties(item)

            model.append(item)

            if (itemStack !== undefined)
                model.setProperty(model.count - 1, "stack", itemStack)
        }


        function setItemWithStack(model, item, index) {
            // This is needed to prevent ListModel.append from converting JsObject to ListModel
            var itemStack = item.stack
            item.stack = undefined
            initDefaultProperties(item)

            model.set(index, item)

            if (itemStack !== undefined)
                model.setProperty(index, "stack", itemStack)
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

            TimeFrameTab {
                width: tabListView.width
                height: tabListView.height
            }
        }

        ListView {
            id: tabListView
            anchors.top: topBar.bottom
            width: parent.width
            height: parent.height - topBar.height - bottomBar.height

            KeyNavigation.tab: topBar
            KeyNavigation.backtab: topBar
            interactive: false
            snapMode: ListView.SnapOneItem
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickDeceleration: 5000
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 240
            highlightRangeMode: ListView.StrictlyEnforceRange
            currentIndex: 1

            model: tabListModel

            onCurrentIndexChanged: {
                if (currentItem && currentItem.grid)
                    currentItem.grid.forceMyFocus()
                topBar.forceActiveFocus()
                mainWindow.currentTabChanged(currentIndex)
                if (currentItem && currentItem.tab)
                    currentItem.tab.gridMouseArea.hideTooltip()
            }

            function processKeyboard(key) {
                if (currentItem)
                    currentItem.tab.processKeyboard(key)
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

        NumberAnimation on opacity { to: 1.0; duration: 500 }

        Component.onCompleted: {
            isCompleted = true
            bottomBar.wheelScroll.connect(tabListView.onWheelScroll)
        }
    } // root
}
