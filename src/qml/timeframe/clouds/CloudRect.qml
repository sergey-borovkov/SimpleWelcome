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

    Item {
        anchors {
            centerIn: parent
        }
        width: Math.min(image.paintedWidth, cloudRect.width)
        height: Math.min(column.height, cloudRect.height)

        Column {
            id: column
            anchors {
                centerIn: parent
                horizontalCenter: parent.horizontalCenter
            }
            Image {
                id: image
                sourceSize.width: cloudRect.width
                sourceSize.height: cloudRect.height - fileName.height
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                cache: false
            }

            Label {
                id: fileName
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                width: cloudRect.width
                text: url.replace(/^.*[\\\/]/, '')
                elide: Text.ElideMiddle
                font.bold: false
                verticalAlignment: Text.AlignTop
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

