import QtQuick 1.1

ListView {
    id: socialView
    spacing: 10
    orientation: ListView.Horizontal

    delegate: ShadowRectangle {
        width: 300
        height: 300

        Text {
            id: msg
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: 200
            text: message
        }

        Image {
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            width: Math.min( sourceSize.width, parent.width)
            height: Math.min( sourceSize.height, parent.height)
            smooth: true
            source: picture
        }
    }
}
