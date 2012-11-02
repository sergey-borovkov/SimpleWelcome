import QtQuick 1.1
import "clouds"

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
                var model = socialDayModel.itemsModel(date)
                cloud.item.cloudDate = date
                cloud.item.model = model
                var c = size - 1
                for(var i = 0; i < cloud.item.children.length; i++) {
                    var subChildren = cloud.item.children[i].children
                    for(var j = 0; j < subChildren.length; j++) {
                        if(subChildren[j].objectName === "cloudRect") {
                            subChildren[j].index = c
                            subChildren[j].model = model
                            subChildren[j].initialize(c)
                            model.updateData.connect(subChildren[j].update)
                            c--
                        }
                    }
                }
            }
        }

        Component.onDestruction: {
            var model = socialDayModel.itemsModel(date)
            var c = size - 1
            for(var i = 0; i < cloud.item.children.length; i++) {
                var subChildren = cloud.item.children[i].children
                for(var j = 0; j < subChildren.length; j++) {
                    if(subChildren[j].objectName === "cloudRect") {
                        model.updateData.disconnect(subChildren[j].update)
                        c--
                    }
                }
            }
        }
    }


    function getSourceComponent(count)
    {

        if (count === 0)
            return ""
        if (count === 1)
            return "clouds/SocialCloudOne.qml"
        else if (count  === 2)
            return "clouds/SocialCloudTwo.qml"
        else if (count === 3)
            return "clouds/SocialCloudThree.qml"
        else if (count > 3)
            return "clouds/SocialCloudFour.qml"
        else
            return "clouds/SocialCloudOne.qml"
    }

}

