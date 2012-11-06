import QtQuick 1.1
import ".."

Rectangle {
    property string url
    property alias image: image

    id: cloudRect
    color: "transparent"
    radius: 10

    function gotThumbnail() {
    }

    Connections {
        target: localDayModel.itemsModel(date)
        onGotThumbnail: {
            if(path === url) {
                image.source += "1"
            }
        }
    }

    ItemRectangle {
        anchors.centerIn: parent
        width: image.paintedWidth - 2
        height: image.paintedHeight - 2
    }

    Image {
        id: image
        anchors.centerIn: parent

        sourceSize.width: parent.width - 10
        sourceSize.height: parent.height - 10

        cache: false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

