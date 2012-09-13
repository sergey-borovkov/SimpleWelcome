import QtQuick 1.0

Item{
    id: cloudTwo
    property date cloudDate
    property variant model
    Item{
        id: cloudRect1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height/2
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item{
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 -20
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
        anchors.top : parent.top
        anchors.left: parent.left
        anchors.right: cloudRect2.left
        anchors.rightMargin: 10
        anchors.bottom: cloudRect1.top

        Text {
            id: dateLabel
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
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
        Qt.createQmlObject('import QtQuick 1.1;  Connections {target: model;  onUpdateData: { \
            rect1.likes = model.likesCount(0); rect1.comments = model.commentsCount(0); \
            rect2.likes = model.likesCount(1); rect2.comments = model.commentsCount(1) \
         } }',cloudTwo);
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
