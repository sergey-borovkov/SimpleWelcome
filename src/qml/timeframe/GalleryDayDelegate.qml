import QtQuick 1.1

Column {
    ItemRectangle {
        id: imageBackground
        width: itemGrid.cellWidth - 20
        height: itemGrid.cellHeight - 40

        Image {
            id: image
            anchors.centerIn: parent
            source: "image://gallery/" + url + "%"
            sourceSize.width: parent.width - 4
            sourceSize.height: parent.height - 4

            Component.onDestruction: {
                galleryPreviewGenerator.cancel(url)
            }

            cache: false
            Connections {
                target: localDayModel.itemsModel(date)
                onGotThumbnail: {
                    if(path === url) {
                        image.source += "1"
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
        text: url
        color: "white"
        elide: Text.ElideLeft
        width: parent.width
        verticalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
