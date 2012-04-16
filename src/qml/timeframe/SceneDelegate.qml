import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height * 2 / 5
    property int count: ( activity == null ? 0 : activity.count )

    width: timeFrameTab.width
    height: timeFrameTab.height

    function getText()
    {
        return "index: " + index +" cur. index " + scene.currentIndex + " date: " +Qt.formatDate( date , "M-yyyy")
    }


    Timer {
        id: timer
        interval: 100; running: true; repeat: true; triggeredOnStart: true
        onTriggered: {
            console.log("date: " + activity.date)
            console.log("count: " + activity.count)
        }
    }

    Text {
        text: getText()
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
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
            anchors.leftMargin: 60
            visible: count >= 1

            activityindex: ( visible ? 0 : -1 )
        }

        CloudDelegate {
            id: cloud2
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            visible: count >= 2
            activityindex: ( visible ? 1 : -1 )
        }

        CloudDelegate {
            id: cloud3
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 60
            visible: count >= 3
            activityindex: ( visible ? 2 : -1 )
        }

    }
}
