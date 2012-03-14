
import QtQuick 1.1

Item {
    id: cloudDelegate

    function createObjects()
    {
        var v = activity

        var y = 0
        var x = 0
        for(var i = 0; i < v.count; i++)
        {
            Qt.createQmlObject("import QtQuick 1.1; Text { y: " + y + ";text:'" + Qt.formatDate(activity.getSetDate(i), "dd-MM-yyyy") + "'}", cloudDelegate, "cloudDelegate")
            y += 50

            for(var j = 0; j < v.getSetCount(i); j++)
            {
                x += 50
                var component = Qt.createComponent("ActivityItem.qml");
                if (component.status === Component.Ready)
                    component.createObject(parent, {"source": "image://preview/" + activity.getUrl(i, j), "y": y, "x": x});
            }
        }
    }

    Component.onCompleted:
    {
        createObjects()
    }

    property int kx: cloudWidth / 27
    property int ky: cloudHeight / 9
    property int x1: 3*kx
    property int x2: 4*kx
    property int x3: 6*kx
    property int y1: 2*ky
    property int y2: 3*ky
    property int y3: 4*ky

    width: cloudWidth
    height: cloudHeight
    anchors.topMargin: (timeFrameTab.height / 2 - height ) / 2
    anchors.bottomMargin:(timeFrameTab.height / 2 - height ) / 2
    anchors.leftMargin: (timeFrameTab.width / 2 - width ) / 2
    anchors.rightMargin: (timeFrameTab.width / 2 - width ) / 2

    Rectangle {
        anchors.fill: parent
        border.color: "blue"
    }
}
