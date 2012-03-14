import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height / 3


    width: timeFrameTab.width
    height: timeFrameTab.height

    Rectangle {
        anchors.fill: parent
        border.color: "green"
        border.width: 3
    }

    Text {
        text: activityListModel.count
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
    }
}
