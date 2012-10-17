import QtQuick 1.1

Item{
    id: cloudOne
    property date cloudDate
    property variant model

    Item {
        id: cloudRect1
        anchors.centerIn: parent
        width: parent.width*3/4
        height: parent.height*3/4
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }

    }

    Text{
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }
}
