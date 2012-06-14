import QtQuick 1.0

Rectangle {
    property alias text: serviceText.text

    height: 50
    color: "#3B5998"

    Text {
        id: serviceText
        font.bold: true
        font.pointSize: 16
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        smooth: true
    }

    Behavior on opacity {
        NumberAnimation { duration: 1000 }
    }
}
