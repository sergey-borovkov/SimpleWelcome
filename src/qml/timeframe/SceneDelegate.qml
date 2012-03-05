import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height / 3

//    width: x3 * 3 + x2 + k * 7
//    height: timeFrameTab.height //y1 + y3 + k * 5

    width: timeFrameTab.width
    height: timeFrameTab.height

    Rectangle {
        anchors.fill: parent
        border.color: "green"
        border.width: 3
    }

    Text {
        text: activityModel.count()
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
    }


    Item {
        id: sceneItem
        anchors.fill: parent

        CloudDelegate {
            id: cloud1
            anchors.top: parent.top
            anchors.left: parent.left
        }
/*
        CloudDelegate {
            id: cloud2
            anchors.top: parent.top
            anchors.right: parent.right
        }

        CloudDelegate {
            id: cloud3
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
        */
    }
}
