import QtQuick 1.1

Rectangle {
    id: commentBox
    property alias edit: edit
    property alias userPhoto: userPhoto
    color: "#EDEFF4"
    radius: 10
    signal send(string comment)
    clip: true
    Image {
        id: userPhoto
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        width: 55        
        anchors.rightMargin: 5
        source: "images/user.png"
    }

    Rectangle {
        id: editBorder
        anchors.left: userPhoto.right
        anchors.right: parent.right
        anchors.top : parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.rightMargin:  5
        color: "white"
        border.color: "#BDC7D8"

        Flickable {
            id: flick
            anchors.fill: parent
            anchors.topMargin: 3
            anchors.leftMargin: 3
            anchors.rightMargin: 3
            anchors.bottomMargin: 3
            contentWidth: edit.paintedWidth
            contentHeight: edit.paintedHeight
            clip: true


            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: edit
                width: flick.width
                height: flick.height
                focus: true
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                Keys.onPressed: {
                    if (edit.text === i18n_Write_Comment)
                    {
                        edit.color = "black"
                        edit.text = ""
                        edit.cursorPosition = 0
                    }
                    if (event.key === Qt.Key_Return) {
                        commentBox.send(edit.text)
                        edit.color = "grey"
                        edit.text = i18n_Write_Comment
                        event.accepted = true
                    }
                }
            }
        }
    }
}

