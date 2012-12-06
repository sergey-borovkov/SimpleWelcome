import QtQuick 1.1

Column {
    spacing: 5

    Item {
        id: imageBackground
        width: itemGrid.cellWidth - 20
        height: itemGrid.cellHeight - 40

        Image {
            id: image

            anchors.centerIn: parent

            source: "image://gallery/" + url
            sourceSize.width: parent.width - 4
            sourceSize.height: parent.height - 4
            cache: false

            Component.onDestruction: {
                galleryPreviewGenerator.cancel(url)
            }

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
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Qt.openUrlExternally(url)
        }
    }
    Text {
        id: label

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width

        text:  url.replace(/^.*[\\\/]/, '')

        style: Text.Raised
        styleColor: "#000"
        color:  "white"
        font.family: "Bitstream Vera Sans"
        font.pointSize: constants.iconTextSize

        elide: Text.ElideMiddle
        maximumLineCount: 3
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
