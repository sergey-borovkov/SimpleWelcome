import QtQuick 1.1

import Qt 4.7

Item {
    id: page
    width: mainWindow.updatableWidth
    height: mainWindow.updatableHeight
    opacity: 0.0

    property bool isCompleted: false

    onWidthChanged: {
        if (isCompleted)
        {
            console.log("resize to " + width + "x" + height)
            welcomeTab.tab.updateGridsContent()
            appsTab.tab.updateGridsContent() // FIX LATER
        }
    }

    onHeightChanged: {
        if (isCompleted)
        {
            console.log("resize to " + width + "x" + height)
            welcomeTab.tab.updateGridsContent()
            appsTab.tab.updateGridsContent() // FIX LATER
        }
    }

    Keys.onPressed: {
        if(event.key == Qt.Key_Escape) {
            event.accepted = true;
            Qt.quit();
        }
    }

    Component.onCompleted: {
        isCompleted = true
        console.log("completed with: " + width + "x" + height + "")
    }

    NumberAnimation on opacity { to: 1.0; duration: 500 }

    function reloadTabs() {
        //console.log("reload here")
        /*welcomeTab.reload();
        appsTab.reload();*/
    }

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: "image://generalicon/asset/bubbles.jpg"
//        opacity: 0.9
        z: -10
    }

    VisualItemModel {
        id: tabListModel

        /*SearchTab {
            id: searchTab
            width: tabListView.width
            height: tabListView.height
        }*/

        Item{}

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
    }

    TopBar {
        id: topBar

        KeyNavigation.tab: tabListView
        KeyNavigation.backtab: tabListView
    }

    ListView {
        id: tabListView

        KeyNavigation.tab: topBar
        KeyNavigation.backtab: topBar

        interactive: false

        anchors.top: topBar.bottom
        anchors.topMargin: 16
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
        onCurrentIndexChanged: {
            if (currentItem && currentItem.grid)
                currentItem.grid.forceActiveFocus()
            topBar.forceActiveFocus()
        }

        function currentTabIndexChanged(newCurrentIndex) {
            tabListView.currentIndex = newCurrentIndex
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
