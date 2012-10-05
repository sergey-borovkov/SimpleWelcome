import QtQuick 1.1

Item {
    property alias model: commentsListView.model
    property alias view: commentsListView

    Component{
        id: commentsDelegate
        Item {
            width: 200; height: 60
            Image
            {
                id: userPhoto
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
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
                color: "grey"
            }
            Text {
                id: textField;
                anchors.left: userPhoto.right
                anchors.top: nameField.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: 10
                text: messageText
                color: "white"
                elide: Text.ElideRight
            }
        }
    }

    ListView {
        id: commentsListView
        anchors.fill: parent
        clip: true
        snapMode:  ListView.SnapToItem
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
