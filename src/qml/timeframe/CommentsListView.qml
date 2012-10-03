import QtQuick 1.1

Item {
    property alias model: commentsListView.model
    property alias view: commentsListView

    Component{
        id: commentsDelegate
        Item {
            width: 354; height: ((textField.paintedHeight + nameField.paintedHeight) > userPhoto.height) ? textField.paintedHeight + nameField.paintedHeight : 55
            Image
            {
                id: userPhoto
                anchors.left: parent.left
                anchors.top: parent.top
                fillMode: Image.PreserveAspectFit
                width: 55
                anchors.rightMargin: 5
                source: fromPictureUrl
            }
            Text {
                id: nameField;
                anchors.left: userPhoto.right
                anchors.top: parent.top
                anchors.leftMargin: 10
                text: from
                color: "white"
            }
            Text {
                id: textField;
                anchors.left: userPhoto.right
                anchors.top: nameField.bottom
                width: parent.width - userPhoto.width - 40
                anchors.leftMargin: 10
                text: messageText
                color: "white"
                wrapMode: Text.Wrap
            }
        }
    }

    ListView {
        id: commentsListView
        anchors.fill: parent
        clip: true
       // snapMode:  ListView.SnapToItem
        property string parentId: ""
        property string pluginName: ""
        delegate: commentsDelegate

        ScrollBar{
            id: scrollBar
            flickable: commentsListView
            vertical: true
            hideScrollBarsWhenStopped: false
        }
    }
}
