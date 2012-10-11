import QtQuick 1.1

Item {
    property int margin: 10
    property int labelHeight: 20

    id: timeLineItem
    height: parent.height
    width: timeLine.width/3

    Item {
        id: cloudBorder
        y: (index%2 === 1)? parent.height/2 + timeScale.height/2 : 0
        x: -100
        width: parent.width + 200
        height: parent.height/2 - timeScale.height/2

        Component.onDestruction: {
            previewGenerator.modelHidden(localDayModel.itemsModel(date))
        }

        Loader {
            id: cloud
            anchors.fill: parent
            source: getSourceComponent(size)
            onLoaded: {
                cloud.item.cloudDate = date
                cloud.item.model = localDayModel.itemsModel(date)
                var c = 0, model = cloud.item.model
                // in CloudSeven.qml cloudrects are nested in another item element
                var objects = (model.count() >= 7) ? cloud.item.children[0].children : cloud.item.children
                var i
                for(i = 0; i < objects.length; ++i) {
                    if('url' in objects[i]) {
                        objects[i].image.source = "image://preview/" + model.url(c) + "/rounded" +"%" + Math.random(10)
                        model.gotThumbnail.connect(objects[i].gotThumbnail)
                        objects[i].url = model.url(c)
                        c++
                    }
                }
                previewGenerator.modelShown(model)
            }
        }
    }

    function getSourceComponent(count)
    {
        if (count === 0)
        {
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
