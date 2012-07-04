import QtQuick 1.1

import Qt 4.7

Item {
    id: page
    width: 1280
    height: 800
    //color: "white"
    opacity: 0.0

    Keys.onPressed: {
        if(event.key == Qt.Key_Escape) {
            event.accepted = true;
            Qt.quit();
        }
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
        opacity: 0.6
        z: -10
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
        z: -5
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

/*        TestingTab {
            id: testingTab
            width: tabListView.width
            height: tabListView.height
        }*/
    }

    TopBar {
        id: topBar
        width: parent.width

        KeyNavigation.tab: tabListView
        KeyNavigation.backtab: tabListView
    }

    Item {
        id: listViewRect
        width: parent.width
        y: topBar.height
        height: parent.height - topBar.height - bottomBar.height
        clip: true
        //color: "transparent"

        ListView {
            id: tabListView

            KeyNavigation.tab: topBar
            KeyNavigation.backtab: topBar
            interactive: false

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            //anchors.topMargin: 16
            width: parent.width
            height: parent.height
            anchors.fill: parent

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
            }

            function currentTabIndexChanged(newCurrentIndex) {
                tabListView.currentIndex = newCurrentIndex
            }

            Component.onCompleted: {
                searchGridModel.currentTabIndexChanged.connect(currentTabIndexChanged)
            }

            //currentIndex: searchGridModel.
        }
    }

    BottomBar {
        id: bottomBar
        width: parent.width

        Component.onCompleted: test();

        function test()
        {
        }
    }

}
