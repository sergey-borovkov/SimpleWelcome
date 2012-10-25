import QtQuick 1.1
import "clouds"

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
            var m = localDayModel.itemsModel(date)
            var l = Math.min(7, m.count())
            for(var i = 0; i < l; ++i) {
                timelinePreviewGenerator.cancel(m.url(i))
            }
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
                        objects[i].url = model.url(c)
                        objects[i].image.source = "image://timeline/" + model.url(c) + "/rounded" +"%" + Math.random(10)
                        c++
                    }
                }
            }
        }
    }

    function getSourceComponent(count)
    {
        if (count === 0)
            return "clouds/CloudNull.qml"
        else if (count === 1)
            return "clouds/CloudOne.qml"
        else if (count === 2)
            return "clouds/CloudTwo.qml"
        else if (count === 3)
            return "clouds/CloudThree.qml"
        else if (count === 4)
            return "clouds/CloudFour.qml"
        else if (count === 5)
            return "clouds/CloudFive.qml"
        else if (count === 6)
            return "clouds/CloudSix.qml"
        return "clouds/CloudSeven.qml"
    }
}
