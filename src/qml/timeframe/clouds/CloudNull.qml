import QtQuick 1.1
import "../"

Item{
    id: cloudNone
    property date cloudDate
    property variant model

    Item {
        anchors.centerIn: parent
        width: parent.width
        height: 200
        Label{
            id: dateLabel
            anchors.top : parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 60
            text: Qt.formatDate( cloudDate , "MMM yyyy")
        }
        AnimatedImage {
            id: waitIndicator
            anchors.top: dateLabel.bottom
            source: "images/ajax-loader.gif"
            anchors.centerIn: parent
        }
    }
}
