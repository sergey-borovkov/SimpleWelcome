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
        x: -100        
        width: parent.width + 200
        height: parent.height/2 - timeScale.height/2 - 10

        Loader {
            id: cloud
            anchors.fill: parent
            source: getSourceComponent(size)
            onLoaded: {
                cloud.item.cloudDate = date
                cloud.item.model = socialDayModel.itemsModel( date )
                cloud.item.createConnection()
            }
        }
    }


    function getSourceComponent(count)
    {

        if (count === 0)
            return ""
        if (count === 1)
            return "SocialCloudOne.qml"
        else if (count  === 2)
            return "SocialCloudTwo.qml"
        else if (count === 3)
            return "SocialCloudThree.qml"
        else if (count > 3)
            return "SocialCloudFour.qml"
        else
            return "SocialCloudOne.qml"
    }

}

