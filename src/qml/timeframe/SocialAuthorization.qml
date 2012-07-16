import QtQuick 1.1

ListView {
    id: socialAuthorization
    delegate: Item {
        Image {
            // get icon for this plugin
            id: img
            width: 200
            height: 200
            source: "image://plugin/" + name
        }
        Text {
            anchors.top: img.bottom
            text: name
            font.pointSize: 24
            color: "white"
        }
    }
}


