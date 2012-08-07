import QtQuick 1.1

FocusScope {
    id: container

    property bool open: false

    Item {
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "#505050"
            focus: true

            /*Text {
                anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; margins: 30 }
                color: "white"
                font.pixelSize: 14
                text: "Second level"
            }*/

            IconGridView {
                id: appsGrid
                dataSource: dataSource_Documents
                anchors.fill: parent

                interactive: false
                focus: true
                keyNavigationWraps: true
                //flow: GridView.TopToBottom

                anchors.top: parent.top
                anchors.topMargin: 32
            }
        }
    }
}
