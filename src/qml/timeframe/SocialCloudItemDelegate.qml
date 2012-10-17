import QtQuick 1.1

Component {
    ShadowRectangle {
        width: cloudItemGridView.cellWidth - 10
        height: cloudItemGridView.cellHeight - 32
        clip: true
        visible: ( index < 8 )

        Behavior on height {
            NumberAnimation {
                duration: 150
            }
        }

        Item {
            id: topLine
            anchors.left: parent.left
            anchors.right: parent.right
            height: Math.max( icon.height, dt.paintedHeight )
            anchors.top: parent.top
            anchors.margins: 6

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

        Item {
            function getHeight() {
                var h = parent.height - topLine.height - 20
                if ( message ) {
                    h -= msg.paintedHeight
                }

                h = picture ? h : 0

                if ( img.sourceSize.height < h ) {
                    h = img.sourceSize.height
                }
                else  if ( h && h > 10 ){
                    h -= 10
                }

                return h
            }

            id: rectImg
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: topLine.bottom
            height: getHeight()

            anchors.topMargin: 6
            anchors.leftMargin: 6
            anchors.bottomMargin: 6
            anchors.rightMargin: 6
//            border.color: "blue"
            Image {
                id: img
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: picture
                width: parent.width
                height: parent.height
            }
        }
        Item {
            id: rectMsg
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: ( picture ) ? rectImg.bottom : topLine.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 6
            anchors.leftMargin: 6
            anchors.bottomMargin: 6
            anchors.rightMargin: 6
//            border.color: "red"
            visible: message

            Text {
                id: msg
                anchors.fill: parent
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: message
                elide: Text.ElideRight
                maximumLineCount: picture ? 1 : 6
            }
        }
    }
}

