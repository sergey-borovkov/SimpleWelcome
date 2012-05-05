import QtQuick 1.1


Item {
    id: sceneDelegate

    property int cloudWidth: timeFrameTab.width * 2 / 5
    property int cloudHeight: timeFrameTab.height * 2 / 5
    property int oldCount: 0

    property variant objects: []
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
        color: "white"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.leftMargin: 10
    }

    Timer {
        id: timer; running: true; repeat: true; interval: 200;
        onTriggered: {

            if(activity.count === oldCount)
                return;

            var v = Qt.createComponent("CloudDelegate.qml");

            // first we delete old items because index items could be changed when new item was added
            // TODO: do it properly without deleting and timer usage

           // console.log("object" + objects.length)
            for( var object in objects) {
                //console.log("Destroying" + object)
                object.opacity = 1
                //object.destroy()
            }

            objects.length = 0

            var settings = [ {"x": 0, "y": 0, "anchors.top": text.bottom, "anchors.left": parent.left, "anchors.leftMargin": 60},
                             {"anchors.bottom": parent.bottom, "anchors.horizontalCenter": parent.horizontalCenter, "idx": 1},
                             {"anchors.top": text.bottom, "anchors.right": parent.right, "anchors.rightMargin": 60, "idx": 2} ]


            for(var i = 0; i < activity.count; i++)
            {                
                var c = v.createObject(sceneDelegate, settings[i]);
                //console.log("Kami" + c)
                objects += c
            }

            oldCount = activity.count

            if(activity.count === 3 || activity.complete) {
                timer.stop()
            }

        }
    }
}
