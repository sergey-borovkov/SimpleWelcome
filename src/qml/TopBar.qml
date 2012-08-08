import QtQuick 1.1

FocusScope {
    id: topBar

    anchors.top: parent.top
    width: parent.width
    height: 80
    focus: true
    Keys.forwardTo: searchInput

    Item {
        id: userIconItem
        width: childrenRect.width
        height: childrenRect.height

        anchors.verticalCenter: parent.verticalCenter
        x: 16

        Image {
            id: userIcon
            source: "image://generalicon/general/usericon"
            width: 64
            height: 64
        }
    }

    Item {
        id: userNameItem
        width: childrenRect.width
        height: childrenRect.height

        anchors.left: userIconItem.right
        anchors.leftMargin: 8

        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: usernameLabel

            maximumLineCount: 1
            elide: Text.ElideRight
            clip: true
            //horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap

            font.pointSize: 18
            font.bold: true
            font.family: "Bitstream Vera Sans"
            style: Text.Raised
            styleColor: "#000"
            color: "#eee"

            Component.onCompleted: {
                text = userInfoProvider.userName
            }

            //text: userInfoProvider.userName
        }

    }

    Item {
        id: searchField
        //color: "white"
        x: Math.max(parent.width/2 - width/2, userNameItem.x + userNameItem.width + 30)
        width: Math.max(40, Math.min(600, lockButton.x - (userNameItem.x + userNameItem.width) - 30*2))
        height: 30

        visible: tabListView.currentIndex !== 3

        anchors.verticalCenter: parent.verticalCenter
        //anchors.horizontalCenter: parent.horizontalCenter

        BorderImage {
            border.left: 10
            border.right: 10
            border.top: 10
            border.bottom: 10
            anchors.fill: parent
            source: "image://generalicon/asset/textfield_border_bg.png"
        }

        TextInput {
            //anchors.horizontalCenter: parent.horizontalCenter
            id: searchInput
            width: parent.width - 24
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            selectByMouse: true
            focus: true
            color: "white"
            font.family: "Bitstream Vera Sans"
            font.italic: true
            font.pixelSize: 18

            /*Keys.onPressed: {
                event.accepted = false
                if (event.key == Qt.Key_Left || event.key == Qt.Key_Right || event.key == Qt.Key_Up || event.key == Qt.Key_Down)
                {
                    console.log("REJECTED")
                    // This is available in all editors.
                    event.accepted = false
                }
                else
                {
                    console.log("ACCEPTED")
                    event.accepted = true
                }
            }*/
        }
        Binding {
            target: searchGridModel
            property: "searchQuery"
            value: searchInput.text
        }
    }

    /*SessionComboBox {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16

        width: 180
        height: 38
    }*/


    // Old Logout and Shutdown buttons
    Item {
        id: lockButton
        width: childrenRect.width
        height: childrenRect.height

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: shutdownButton.left
        //anchors.rightMargin: 8
        anchors.rightMargin: 0

        Image {
            id: lockIcon
            source: "image://generalicon/asset/button_lock.png"
            width: 47
            height: 41
        }

        MouseArea {
            id: lockButtonMouseArea

            anchors.fill: parent

            onClicked: {
                sessionProvider.lock();
            }

        }
    }

    Item {
        id: shutdownButton
        width: childrenRect.width
        height: childrenRect.height

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16

        Image {
            id: shutdownIcon
            source: "image://generalicon/asset/button_shutdown.png"
            width: 46
            height: 41
        }

        MouseArea {
            id: shutdownButtonMouseArea

            anchors.fill: parent

            onClicked: {
                sessionProvider.shutdown();
            }

        }
    }
}
