import QtQuick 1.1

Item{
    id: cloudOne
    property date cloudDate
    property variant model

    Item {
        id: cloudRect1
        anchors.centerIn: parent
        width: parent.width*3/4
        height: parent.height*3/4
        SocialCloudItem {
            id: rect1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width: parent.width
            height: parent.height
        }

    }

    Text{
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    function createConnection()
    {
        rect1.id = model.id(0)
        rect1.message = model.text(0)
        rect1.picture = model.imageUrl(0)
        rect1.like = model.like(0)
        rect1.likes = model.likesCount(0)
        rect1.commentCount = model.commentsCount(0)
        rect1.pluginName = model.pluginName(0)
        rect1.iconPlugin.source =  "image://plugin/" + rect1.pluginName + "/small"
        rect1.commentsView.model = model.comments(0)
        Qt.createQmlObject('import QtQuick 1.1;  Connections {target: model; onUpdateData: { \
            rect1.likes = model.likesCount(0); rect1.commentCount = model.commentsCount(0); rect1.like = model.like(0) } }',cloudOne);
    }
}
