import QtQuick 1.1

ListView {
    delegate: Item {
        width: Math.max( img.width, txt.paintedWidth ) + 10
        height: img.height + txt.height + 10

        Image {
            id: icon
            anchors.bottom: img.bottom
            anchors.right: img.right
            width: 32
            height: 32
            z: 1
            source: authorized ? "images/green_icon.png" : ""

        }
        Image {
            // get icon for this plugin
            id: img
            smooth: true
            z: -1
            width: 128
            height: 128
            source: "image://plugin/" + name
        }
        Text {
            id: txt
            anchors.top: img.bottom
            text: name
            font.pointSize: 24
            color: "white"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pluginModel.logout(index)
            }
        }
    }
}
