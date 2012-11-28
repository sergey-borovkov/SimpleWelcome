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

    Column {
        spacing: 5
        anchors.centerIn: parent

        Item {
            width: image.paintedWidth - 2
            height: image.paintedHeight - 2
            anchors.horizontalCenter: parent.horizontalCenter

            ItemRectangle {
                anchors.fill: parent
            }

            Image {
                id: image
                sourceSize.width: cloudRect.width - 10
                sourceSize.height: cloudRect.height - 10

                cache: false
            }
        }

        Text {
            id: fileName
            anchors.horizontalCenter: parent.horizontalCenter
            width: cloudRect.width

            text: url.replace(/^.*[\\\/]/, '')

            style: Text.Raised
            styleColor: "#000"
            color:  "white"
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize

            elide: Text.ElideMiddle
//            maximumLineCount: 2
//            wrapMode: Text.Wrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

