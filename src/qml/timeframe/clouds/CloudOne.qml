import QtQuick 1.1
import "../"

Item {
    id: cloudOne
    property date cloudDate
    property variant model

    Label {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
    }

    CloudRect {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        width: parent.width * 1 / 3
        height: parent.height * 4 / 5
    }

}
