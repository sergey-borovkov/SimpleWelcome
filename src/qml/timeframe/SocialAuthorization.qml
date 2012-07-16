import QtQuick 1.1

ListView {
    id: socialAuthorization
    delegate: Item {
        anchors.fill: parent
        Image {
            // get icon for this plugin
            id: img
            smooth: true
            width: 128
            height: 128
            source: "image://plugin/" + name
        }
        Text {
            anchors.top: img.bottom
            text: name
            font.pointSize: 24
            color: "white"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: { parent.color = "white"}
        }
    }
}


