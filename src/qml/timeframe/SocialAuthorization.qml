import QtQuick 1.1
import ".."

ListView {
    spacing: 15
    delegate: Item {
        //width: Math.max(img.width,  ) + 10
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
        }
        TabButton {
            id: button
            active: true
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
