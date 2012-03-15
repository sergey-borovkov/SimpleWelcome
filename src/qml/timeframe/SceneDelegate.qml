import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height * 2 / 5


    width: timeFrameTab.width
    height: timeFrameTab.height

    Component.onCompleted:
    {
//        console.log( "sceneDelegate is loaded")
//        createObjects()
    }

/*
    Rectangle {
        anchors.fill: parent
        border.color: "green"
        border.width: 3
    }
*/
    Text {
        text: "index: " + activityListModel.count
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.left: parent.left
        anchors.leftMargin: 10
    }


    Item {
        id: sceneItem
        anchors.fill: parent

        CloudDelegate {
            id: cloud1
            x: 0
            y: 0
            anchors.top: parent.top
            anchors.left: parent.left
            visible: ( activity.count >= 1 )

            activityindex: ( visible ? 0 : -1 )
        }

        CloudDelegate {
            id: cloud2
//            x: ( timeFrameTab.width - cloudWidth )
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            visible: ( activity.count >= 2 )
            activityindex: ( visible ? 1 : -1 )
        }

        CloudDelegate {
            id: cloud3
            anchors.top: parent.top
            anchors.right: parent.right
            visible: ( activity.count >= 3 )
            activityindex: ( visible ? 2 : -1 )
        }
    }
}
