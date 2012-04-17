import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height * 2 / 5

    property int cl1: 0
    property int cl2: 0
    property int cl3: 0

    width: timeFrameTab.width
    height: timeFrameTab.height

    function getText()
    {
        return "index: " + index +" cur. index " + scene.currentIndex + " date: " +Qt.formatDate( date , "M-yyyy")
    }

    Component.onCompleted: {
        timer.start()
    }

    Text {
        id: text
        text: getText()
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.leftMargin: 10
    }

/*
    Component.onDestruction: {
        timer.stop()
    }
*/
    Timer {
        id: timer
        interval: 200; running: true; repeat: true;
        onTriggered: {
            var v = Qt.createComponent("CloudDelegate.qml");

            if(activity.count >= 1 && !cl1) {
                cl1 = 1;
                console.log("Creating cloud1")
                var c = v.createObject(sceneDelegate, {"x": 0, "y": 0, "anchors.top": text.bottom, "anchors.left": parent.left, "anchors.leftMargin": 60})
                c.idx = 0
            }

            if(activity.count >= 2 && !cl2) {
                cl2 = 1;
                console.log("Creating cloud2")
                var c = v.createObject(sceneDelegate, {"anchors.bottom": parent.bottom, "anchors.horizontalCenter": parent.horizontalCenter, "idx": 1});
                c.idx = 1
            }

            if(activity.count >= 3 && !cl3) {
                cl3 = 1;
                console.log("Creating cloud3")
                var c = v.createObject(sceneDelegate, {"anchors.top": parent.top, "anchors.right": parent.left, "anchors.rightMargin": 60, "idx": 2});
                c.idx = 2
            }

            if(activity.count === 3) {
                timer.stop()
            }
        }
    }

}
