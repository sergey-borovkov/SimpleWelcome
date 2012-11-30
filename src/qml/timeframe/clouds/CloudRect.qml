import QtQuick 1.1
import ".."

Item {
    id: cloudRect

    property string url
    property alias image: image

    Connections {
        target: localDayModel.itemsModel(date)
        onGotThumbnail: {
            if(path === url) {
                image.source += "1"
            }
        }
    }

    Item {
        id: imageWrapper
        width: cloudRect.width
        height: cloudRect.height - fileName.font.pixelSize - 5

        Image {
            id: image
            anchors {
                centerIn: parent
                verticalCenterOffset: -2
            }
            sourceSize {
                width: parent.width
                height: parent.height
            }

            cache: false
        }
    }
    Label {
        id: fileName
        anchors.bottom: parent.bottom
        width: image.sourceSize.width

        text: url.replace(/^.*[\\\/]/, '')
        elide: Text.ElideMiddle
        font.bold: false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

