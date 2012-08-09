import QtQuick 1.1

Item {
    property int margin: 10
    property int labelHeight: 20

    id: timeLineItem
    height: parent.height
    width: timeLine.width/3

    // using Rectangle because with Item there is painting bug...
    Rectangle {
        id: cloudBorder
        color: "transparent"
        y: (index%2 === 1) ? parent.height/2 + timeScale.height/2 : 0
        anchors.leftMargin: -50
        width: parent.width + 100
        height: parent.height/2 - timeScale.height/2 - 10
        clip: true
        GridView{
            id: cloudItemGridView
            anchors.fill: parent
            cellWidth: parent.width / 4
            cellHeight: parent.height / 2
            interactive: false
            anchors.margins: 5

            header: Text { text: "<b>" + date.getDate() + "-" + date.getMonth() + "-" + date.getFullYear() + "</b>"; color: "white" }

            model: socialDayModel.itemsModel( date )
            delegate: SocialCloudItemDelegate { }
        }

    }



}
