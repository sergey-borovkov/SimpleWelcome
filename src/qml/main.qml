import QtQuick 1.0

import Qt 4.7

import "./timeframe/" 1.0

Rectangle {
    id: page
    width: 1280
    height: 800
    color: "white"

    VisualItemModel {
        id: tabListModel

        /*
        FirstTab {
            width: tabListView.width
        }

        SecondTab {
            width: tabListView.width
        }
*/
        TimeFrameTab {
            width: tabListView.width
            height: tabListView.height
        }
    }

    Rectangle {
        id: listViewRect
        width: parent.width
        height: parent.height - tabBar.height
        clip: true
        
        ListView {
            id: tabListView

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            //anchors.topMargin: 16
            width: parent.width
            height: parent.height

            model: tabListModel

            snapMode: ListView.SnapOneItem
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickDeceleration: 5000
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 240
            highlightRangeMode: ListView.StrictlyEnforceRange
        }
    }

    Rectangle {
        id: tabBar

        anchors.bottom: parent.bottom
        width: parent.width
        height: 80
        color: "grey"

        Row {
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            spacing: 40

            TabButton {
                label: "First Tab"
                id: firstTabButton
                onButtonClick: tabListView.currentIndex = 0
            }

            TabButton {
                label: "Second Tab"
                id: secondTabButton
                onButtonClick: tabListView.currentIndex = 1
            }

            TabButton {
                label: "TimeFrame"
                id: thirdTabButton
                onButtonClick: tabListView.currentIndex = 2
            }

        }
    }
}
