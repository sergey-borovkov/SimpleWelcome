import QtQuick 1.0

Item{
    id: cloudSix
    property date cloudDate
    property variant model
    property int margin: 10
    property variant __children: children

    Item {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: parent.height/6
        anchors.left: parent.left
        height: parent.height/10
        width: parent.width/3
        Text{
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            horizontalAlignment: Text.AlignHCenter

            style: Text.Raised
            styleColor: "#000"
            color:  "white"
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize
            font.bold: true
        }
    }

    CloudRect {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        width: dateLabel.width
        height: parent.height*3/5
    }

    CloudRect {
        id: cloudRect2
        anchors.top: parent.top
        anchors.left: cloudRect1.right
        anchors.leftMargin: margin
        height: parent.height*5/9
        width: cloudRect1.width
    }

    CloudRect {
        id: cloudRect3
        anchors.left: cloudRect2.right
        anchors.leftMargin: margin
        anchors.bottom: cloudRect2.bottom
        width: parent.width/4
        height: parent.height*4/9
    }

    CloudRect {
        id: cloudRect4
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.right
        anchors.leftMargin: margin
        width: parent.width*2/9
    }

    CloudRect {
        id: cloudRect5
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect4.right
        anchors.leftMargin: margin
        width: cloudRect4.width
    }

    CloudRect {
        id: cloudRect6
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect5.right
        anchors.leftMargin: margin
        width: cloudRect4.width
    }
}
