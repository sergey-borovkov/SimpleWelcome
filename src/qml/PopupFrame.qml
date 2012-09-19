import QtQuick 1.1

FocusScope {
    id: container
    clip: true

    property alias gridGroup: appsGrid
    property alias arrowX: arrow.x
    property alias groupTitle: appsGrid.groupName
    //property bool open: false

    Item {
        width: parent.width
        height: childrenRect.height + 3

        Image {
            id: arrow
            source: "image://generalicon/asset/popup_arrow.png"
            anchors.bottom: groupBorder.top
            anchors.bottomMargin: -1
            z: 1
        }

        Rectangle {
            id: groupBorder
            x: -1
            width: parent.width + 1
            anchors.top: parent.top
            anchors.topMargin: 14
            height: childrenRect.height + 64
            //anchors.fill: parent
            color: "#505050"
            focus: true

            border.color: "#A0A0A0"

            /*Text {
                anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; margins: 30 }
                color: "white"
                font.pixelSize: 14
                text: "Second level"
            }*/

            GridWithGroup {
                id: appsGrid
                width: parent.width
                height: childrenRect.height

                anchors.top: parent.top
                anchors.topMargin: 32

                draggable: true
                //groupNameVisible:
            }
        }
    }
}
