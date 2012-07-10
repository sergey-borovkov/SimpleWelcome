import QtQuick 1.1

Rectangle {
    //property string imageUrl: image.source
    property string url
    property variant image: image

    id: cloudRect
    color: "black"
    radius: 10

    Image{
        id: image
        anchors.centerIn: parent
        width: Math.min( sourceSize.width, parent.width -10)
        height: Math.min( sourceSize.height, parent.height -10 )
        fillMode: Image.PreserveAspectFit
        smooth: true
        asynchronous: true
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(url)
        }
    }
}

