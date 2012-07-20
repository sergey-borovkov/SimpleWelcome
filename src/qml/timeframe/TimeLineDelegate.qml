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
        Loader {
            id: cloud
            anchors.fill: parent
            source: getSourceComponent(size)
            onLoaded: {
                   cloud.item.cloudDate = date
                   cloud.item.model = galleryModel.itemsModel(date)
                   cloud.item.createConnection()
            }
        }
    }

    function getSourceComponent(count)
    {
        if (count === 0)
        {
            galleryLister.startSearch(date,1)   //Start new search if null item becomes visible
            return "CloudNull.qml"
        }
        else if (count === 1)
            return "CloudOne.qml"
        else if (count === 2)
            return "CloudTwo.qml"
        else if (count === 3)
            return "CloudThree.qml"
        else if (count === 4)
            return "CloudFour.qml"
        else if (count === 5)
            return "CloudFive.qml"
        else if (count === 6)
            return "CloudSix.qml"
        return "CloudSeven.qml"
    }

}
