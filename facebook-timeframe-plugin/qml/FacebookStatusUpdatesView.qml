import QtQuick 1.0

ListView {
    id: statusListComponent
    delegate: listDelegate
    anchors.margins: 15

    Component {
        id: listDelegate
        Rectangle {
            height: 70
            width: 750
            border.color: "darkgrey"
            border.width: 1

            Image {
                id: avatar
                source: userPictureUrl
                height: parent.height - 10
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: statusText

                anchors.margins: 10
                anchors.left: avatar.right
                anchors.top:  parent.top
                font.family: "Arial"
                font.pointSize: 12
                text: statusMessage
                width: 700
                wrapMode: Text.WordWrap
            }

            Text {
                id: createdText

                anchors.leftMargin: 10
                anchors.topMargin: 5
                anchors.left: avatar.right
                anchors.top: statusText.bottom
                font.family: "Arial"
                font.pointSize: 9
                color: "lightgrey"
                text: created
            }
        }
    }
}
