import QtQuick 1.1

Column {
    ItemRectangle {
        id: imageBackground
        width: itemGrid.cellWidth - 20
        height: itemGrid.cellHeight -40
        Image {
            id: image
            anchors.centerIn: parent
            width: Math.min( sourceSize.width, parent.width -4)
            height: Math.min( sourceSize.height, parent.height -4 )
            fillMode: Image.PreserveAspectFit
            source: "image://gallery/" + url + "%"
            smooth: true
            asynchronous: true

            // because gridview for some reason instantiates
            // delegates for whole model when it's created
            // following is very slow and should be later replaced
            // with more proper solution

            Component.onCompleted: {
                localDayModel.itemsModel(date).gotThumbnail.connect(gotThumbnail)
                galleryPreviewGenerator.itemShown(url, localDayModel.itemsModel(date))
            }

            Component.onDestruction: {
                galleryPreviewGenerator.itemHidden(url, localDayModel.itemsModel(date))
            }

            function gotThumbnail(path)
            {
                if(path === url) {
                    source += "1"
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
