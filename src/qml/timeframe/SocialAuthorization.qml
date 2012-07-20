import QtQuick 1.1

ListView {
    delegate: Item {
//        anchors.centerIn: parent
        width: Math.max( img.width, txt.paintedWidth ) + 10
        height: img.height + txt.height + 10

        Image {
            // get icon for this plugin
            id: img
            smooth: true
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
                pluginModel.show(index)
            }
        }
    }
}


