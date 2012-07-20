import QtQuick 1.1

Component {
    id: desu
    ShadowRectangle {
        width: 200
        height: Math.max( 120, ( column.height + bottomLine.height ) )
        Column {
            id: column
            width: parent.width
            anchors.topMargin: 2
            anchors.bottomMargin: 10
            anchors.top: parent.top
            spacing: 10

            Item {
                width: parent.width
                height: Math.max( icon.height, dt.paintedHeight )
                Image {
                    id: icon
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    width: 32
                    height: 32
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "image://plugin/" + pluginName
                }
                Text {
                    id: dt
                    anchors.left: icon.right
                    anchors.right: parent.right
                    anchors.rightMargin: 3
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    text: date
                    color: "grey"
                }
            }
            Image {
                id: img
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                smooth: true
                source: picture
            }
            Text {
                id: msg
                width: parent.width
                anchors.bottomMargin: 3
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: message
            }

            Text {
                id: audioItem
                width: parent.width
                anchors.bottomMargin: 3
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Audio: " + audio
                color: "lightblue"
                visible: audio !== ""
            }

        }
        Item {
            id: bottomLine
            width: parent.width
            height: Math.max( commentsCount.paintedHeight, dt.paintedHeight )
            anchors.bottom: parent.bottom
            anchors.topMargin: 3

            Text {
                id: commentsCount
                anchors.left: parent.left
                anchors.leftMargin: 3
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                text: "Comments: " + comments
                color: "grey"
                visible: comments !== ""
            }
            Text {
                id: likesCount
                anchors.left: commentsCount.right
                anchors.right: parent.right
                anchors.rightMargin: 3
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: "Likes: " + likes
                color: "grey"
                visible: likes !== ""
            }
        }
    }
}

