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
                var oldSource = image.source
                image.source = ""
                image.source = oldSource
            }
        }
    }

    Item {
        id: imageWrapper
        width: cloudRect.width
        height: cloudRect.height - fileName.font.pixelSize
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: image
            fillMode: Image.PreserveAspectFit
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            y: imageWrapper.y + (imageWrapper.height - height)/2 - fileName.font.pixelSize + 20

            sourceSize {
                width: parent.width
                height: parent.height
            }

            cache: false
        }
    }
    Label {
        id: fileName
        anchors.horizontalCenter: parent.horizontalCenter
        y: image.y + image.paintedHeight - 15
        width: parent.width

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

