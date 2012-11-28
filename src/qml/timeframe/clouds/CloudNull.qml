import QtQuick 1.1

Item{
    id: cloudNone
    property date cloudDate
    property variant model

    Item {
        anchors.centerIn: parent
        width: parent.width
        height: 200
        Text{
            id: dateLabel
            anchors.top : parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 60
            text: Qt.formatDate( cloudDate , "MMM yyyy")
            horizontalAlignment: Text.AlignHCenter

            style: Text.Raised
            styleColor: "#000"
            color:  "white"
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize
            font.bold: true
        }
        AnimatedImage {
            id: waitIndicator
            anchors.top: dateLabel.bottom
            source: "images/ajax-loader.gif"
            anchors.centerIn: parent
        }
    }
}
