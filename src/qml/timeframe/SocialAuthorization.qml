import QtQuick 1.1
import ".."

PathView {
    anchors.fill: parent
    path: Path {
        startX: 0
        startY: height/2
    }

    delegate: Item {
        anchors.fill: parent

        Item {
            anchors.centerIn: parent
            width: childrenRect.width
            height: img.height + button.height + 10
            Image {
                // get icon for this plugin
                id: img
                smooth: true
                z: -1
                width: 128
                height: 128
                source: "image://plugin/" + name
                anchors.horizontalCenter: button.horizontalCenter
            }
            TabButton {
                id: button
                active: false
                anchors.top: img.bottom
                anchors.left: img.left
                anchors.right: img.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                height: 28
                label: authorized ? "Logout" : "Login"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        pluginModel.logout(index)
                    }
                }
            }
        }
    }
}
