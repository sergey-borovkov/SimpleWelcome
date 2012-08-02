import QtQuick 1.1

Item {    
    property int margin: 10
    property int labelHeight: 20

    id: timeLineItem
    height: parent.height
    width: timeLine.width/3


    /*
    Rectangle{
        id: gridBorder
        border.color: "white"
        color: "transparent"
        border.width: 2
        anchors.top: parent.top
        anchors.left: dateLabel.left
        width: parent.width
        height: parent.height-40
        //visible: (size === 0) ? false : true
    }
    */
    Item {
        id: cloudBorder
        //border.color: "white"
        //color: "transparent"
        y: (index%2 === 1)? parent.height/2 + timeScale.height/2 : 0
        anchors.leftMargin: -50
        //anchors.rightMargin: -25
        width: parent.width + 100
        height: parent.height/2 - timeScale.height/2
        clip: true
        GridView{
            anchors.fill: parent
            cellWidth: parent.width / 4
            cellHeight: parent.height / 2
            model: socialDayModel.itemsModel(date)
            delegate: SocialDelegate { }
        }

    }



}
