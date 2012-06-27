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
            if(activity.complete) {
                timer.stop()
            }

            if(activity.count === oldCount)
                return;

            // first we make old items invisible. then create all items again.
            // it's most simple solution, it would be better for this to delete objects
            // but for some reason calling destroy() does not work...
            // TODO: do it properly without deleting and timer

            for(var i = 0; i < objects.length; i++) {
                objects[i].visible = false
            }

            objects = []

            var settings = [ {"x": 0, "y": 0, "anchors.top": text.bottom, "anchors.left": parent.left, "anchors.leftMargin": 60},
                             {"anchors.bottom": parent.bottom, "anchors.horizontalCenter": parent.horizontalCenter, "idx": 1},
                             {"anchors.top": text.bottom, "anchors.right": parent.right, "anchors.rightMargin": 60, "idx": 2} ]

            var component = Qt.createComponent("CloudDelegate.qml");

            for(var i = 0; i < activity.count; i++)
            {                
                var cloud = component.createObject(sceneDelegate, settings[i]);

                var arr = objects;
                arr.push(cloud)
                objects = arr
            }

            oldCount = activity.count
        }
    }
}
