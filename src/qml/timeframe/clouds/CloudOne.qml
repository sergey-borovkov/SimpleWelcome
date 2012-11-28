import QtQuick 1.1

Item {
    id: cloudOne
    property date cloudDate
    property variant model

    Text {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
        horizontalAlignment: Text.AlignHCenter

        style: Text.Raised
        styleColor: "#000"
        color:  "white"
        font.family: "Bitstream Vera Sans"
        font.pointSize: constants.iconTextSize
        font.bold: true
    }

    CloudRect {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
    }

}
