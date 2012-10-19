import QtQuick 1.0
import ".."
Item{
    id: cloudThree
    property date cloudDate
    property variant model
    property int margin: 10


    Item {
        id: cloudRect1
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height*3/5
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 - margin
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
        id: cloudRect3
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.left
        anchors.rightMargin: margin
        width: parent.width*1/4
        SocialCloudItem {
            id: rect3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        anchors.top : cloudRect2.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: cloudRect3.right
        anchors.leftMargin: margin

        Text{
            id: dateLabel
            anchors.centerIn: parent
            width: 100
            height: labelHeight
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
   }
}
