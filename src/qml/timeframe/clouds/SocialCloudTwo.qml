import QtQuick 1.0
import ".."
Item{
    id: cloudTwo
    property date cloudDate
    property variant model
    Item{
        id: cloudRect1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height/2
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item{
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 -20
        height: parent.height*4/5
        SocialCloudItem {
            id: rect2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }
    Item {
        anchors.top : parent.top
        anchors.left: parent.left
        anchors.right: cloudRect2.left
        anchors.rightMargin: 10
        anchors.bottom: cloudRect1.top

        Text {
            id: dateLabel
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
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
    }
}
