import QtQuick 1.1

Item {
    property alias view: msgView
    property alias scrollBar: msgScrollBar
    property alias socialMessage: socialMessage
    property alias msgMouseArea: msgMouseArea
    Flickable {
        id: msgView
        width: parent.width
        height: socialMessage.paintedHeight
        contentHeight: socialMessage.paintedHeight

        clip: true

        Text {
            id: socialMessage
            width: msgView.width - 20
            anchors {
                bottomMargin: 3
                horizontalCenter: parent.horizontalCenter
            }

            onLinkActivated: {
                Qt.openUrlExternally(link)
            }

            wrapMode: Text.Wrap
            horizontalAlignment: truncated ? Text.AlignLeft : Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: (picture === "") ? message : ""
            color: "white"
            clip: true
            textFormat: Text.StyledText
            elide: Text.ElideRight
            maximumLineCount: {
                var lines = (mainRect.height - topLine.height - fromItem.height -
                             (audioItem.visible ? audioItem.height : 0) -
                             (videoItem.visible ? videoItem.height : 0)) / font.pixelSize / 1.5;
                return lines
            }

            MouseArea {
                id: msgMouseArea
                anchors.fill: parent

                onClicked: {
                    popupDetailsWidget()
                }

                hoverEnabled: true
            }
        }
    }

    ScrollBar {
        id: msgScrollBar
        flickable: msgView
        vertical: true
        hideScrollBarsWhenStopped: false
        visible: false
    }
}
