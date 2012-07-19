import QtQuick 1.1

Component {
    id: desu
    ShadowRectangle {
        width: 200
        height: Math.max(120, childrenRect.height)
        Column {
            id: column
            width: parent.width
            anchors.topMargin: 2
            anchors.bottomMargin: 10
            anchors.top: parent.top
            spacing: 10

            Image {
                id: icon
                anchors.leftMargin: 3
                width: 32
                height: 32
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "image://plugin/" + pluginName
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
        }
    }
}

