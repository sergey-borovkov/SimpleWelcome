import QtQuick 1.0

Item{
    id: cloudFour
    property date cloudDate
    property variant model
    property int margin: 10

    Item {
        id: cloudRect1
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width*4/7
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
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: parent.height/10
        Text{
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Item {
        id: cloudRect2
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: cloudRect1.height - dateLabel.height
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
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.right
        width: parent.width*2/7
        SocialCloudItem {
            id: rect3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: cloudRect4
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.left
        width: cloudRect3.width
        SocialCloudItem {
            id: rect4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }
    }

    function createConnection()
    {
        setProperties(rect1, 0)
        setProperties(rect2, 1)
        setProperties(rect3, 2)
        setProperties(rect4, 3)
    }

    function setProperties(item, index)
    {
        item.message = model.text(index)
        item.picture = model.imageUrl(index)
        item.likes = model.likesCount(index)
        item.comments = model.commentsCount(index)
        item.pluginName = model.pluginName(index)
        item.pluginIcon.source =  "image://plugin/" + item.pluginName
        item.commentsView.model = model.comments(0)
    }
}