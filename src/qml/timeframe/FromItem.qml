import QtQuick 1.1

Item {
    property alias userImage: fromImage.source
    property alias userName: fromName.text

    height: fromImage.height
    Image {
        id: fromImage
        source: "images/user.png"
        width: 24
        height: 24
        anchors {
            left: parent.left
            leftMargin: 5
            rightMargin: 5
        }

        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: fromName
        color: "white"
//        font.pointSize: 9
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors {
            left: fromImage.right
            right: parent.right
            leftMargin: 5
            rightMargin: 5
        }
        elide: Text.ElideRight
    }
}
