import QtQuick 1.1

Rectangle {
    property string url
    property alias image: image

    id: cloudRect
    color: "transparent"
    radius: 10

    function gotThumbnail(newUrl) {
        if(newUrl === url) {
            image.source = image.source + "1"
        }
    }
    ItemRectangle {
        anchors.centerIn: parent
        width: image.paintedWidth -1
        height: image.paintedHeight -1
    }

    Image {
        id: image
        anchors.centerIn: parent
        width: Math.min(sourceSize.width, parent.width -10)
        height: Math.min(sourceSize.height, parent.height -10 )
        fillMode: Image.PreserveAspectFit
        smooth: true
        asynchronous: true
        cache: false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

