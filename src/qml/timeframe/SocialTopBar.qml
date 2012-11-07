import QtQuick 1.1

Item {
    id: topLine

    property alias pluginIcon : iconPlugin.source
    property alias dateText :   dateArea.text
    property alias likesCount : likesCountText.text
    property alias commentsCount:  commentsCountText.text

    signal exitClicked

    Image {
        id: iconPlugin

        anchors { top: parent.top; left: parent.left; leftMargin: 10 }
        width: 22
        height: 22

        fillMode: Image.PreserveAspectFit
        smooth: true
    }
    Text {
        id: dateArea

        anchors.centerIn: parent

        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        color: "lightgrey";
        visible: false
    }
    Item {
        id: likesCountArea

        anchors { top: parent.top; right: parent.right; leftMargin: 5; topMargin: 3 }
        width: 40
        height: 22

        Image {
            id: smallLikeIcon
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 22
            height: 22

            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "images/like.png"
        }
        Text {
            id: likesCountText

            anchors { left: smallLikeIcon.right; verticalCenter: parent.verticalCenter; leftMargin: 5 }

            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }
    }
    Item {
        id: commentsCountArea

        anchors { right: likesCountArea.left; top: parent.top; rightMargin: 5; topMargin: 3 }
        width: 40
        height: 22

        Image {
            id: smallCommentIcon

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 22
            height: 22

            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "images/comment.png"
        }
        Text {
            id: commentsCountText

            anchors.left: smallCommentIcon.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 5

            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }
    }

    Item {
        id: detailsOffArea

        anchors {verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 5 }
        width: 12
        height: 12

        visible: false

        Image{
            id: closeIcon

            anchors.centerIn: parent

            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "images/x.png"
            width: 8
            height: 8
        }
        MouseArea{
            anchors.fill: parent

            hoverEnabled: true
            onClicked: {
                exitClicked()
            }
            onEntered: {
                closeIcon.width = 10
                closeIcon.height = 10
            }
            onExited:{
                closeIcon.width = 8
                closeIcon.height = 8
            }

        }
    }
    states: [
        State {
            name: "detailed"
            PropertyChanges { target: detailsOffArea; visible: true }
            PropertyChanges { target: likesCountArea; visible: false }
            PropertyChanges { target: commentsCountArea; visible: false }
            PropertyChanges { target: dateArea; visible: true }
        }
    ]
}
