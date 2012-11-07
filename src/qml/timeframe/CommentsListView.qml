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

                width: 55
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.rightMargin: 5

                fillMode: Image.PreserveAspectFit                
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
                anchors.leftMargin: 10
                width: parent.width - userPhoto.width - 40

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
