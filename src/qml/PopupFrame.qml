import QtQuick 1.1

FocusScope {
    id: container

    property alias gridGroup: appsGrid
    //property bool open: false

    Item {
        anchors.fill: parent

        Rectangle {
            x: -1
            width: parent.width + 1
            height: parent.height-1
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

                groupName: "Popup group"
                draggable: true
                //groupNameVisible:
            }
        }
    }
}
