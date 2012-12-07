import QtQuick 1.1

Item {
    property alias view: msgView
    property alias scrollBar: msgScrollBar
    property alias socialMessage: socialMessage

    Flickable {
        id: msgView
        width: parent.width
        height: parent.height
        contentHeight: socialMessage.height
        contentWidth: socialMessage.width
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Text {
            id: socialMessage
            width: msgView.width
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
            textFormat: Text.StyledText
            elide: Text.ElideRight
            maximumLineCount: {
                var h = msgView.parent.height - 10
                h -= (socialImage.height - 10) * socialImage.visible
                h -= (audioItem.height - 10) * audioItem.visible
                h -= (videoItem.height - 10) * videoItem.visible
                var lines = h / font.pixelSize / 1.5;
                return lines
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
