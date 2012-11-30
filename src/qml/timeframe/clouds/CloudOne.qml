import QtQuick 1.1
import "../"

Item {
    id: cloudOne
    property date cloudDate
    property variant model
    property int margin: 10

    Label {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: margin
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
    }

    CloudRect {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        width: parent.width
        height: parent.height * 4 / 5
    }

}
