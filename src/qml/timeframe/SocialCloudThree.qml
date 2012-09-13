import QtQuick 1.0

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
    function createConnection()
    {
        setProperties(rect1, 0)
        setProperties(rect2, 1)
        setProperties(rect3, 2)
        Qt.createQmlObject('import QtQuick 1.1;  Connections {target: model;  onUpdateData: { \
            rect1.likes = model.likesCount(0); rect1.comments = model.commentsCount(0); \
            rect2.likes = model.likesCount(1); rect2.comments = model.commentsCount(1); \
            rect3.likes = model.likesCount(2); rect3.comments = model.commentsCount(2) \
         } }',cloudThree);
    }

    function setProperties(item, index)
    {
        item.id = model.id(index)
        item.message = model.text(index)
        item.picture = model.imageUrl(index)
        item.likes = model.likesCount(index)
        item.comments = model.commentsCount(index)
        item.pluginName = model.pluginName(index)
        item.pluginIcon.source =  "image://plugin/" + item.pluginName
        item.commentsView.model = model.comments(index)
    }
}
