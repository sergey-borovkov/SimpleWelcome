import QtQuick 1.1

FocusScope {
    id: topBar

    property alias searchText: searchInput.text
    property bool largeVariant: root.height > 800

    anchors.top: parent.top
    width: parent.width
    height: largeVariant ? 80 : 60
    focus: true
    Keys.forwardTo: [searchInputFilter, searchInput]

    Item {
        id: userIconItem
        width: childrenRect.width
        height: childrenRect.height

        anchors.verticalCenter: parent.verticalCenter
        x: 16

        Image {
            id: userIcon
            source: "image://generalicon/general/usericon"
            sourceSize.width: topBar.height - 16
            sourceSize.height: topBar.height - 16
            smooth: true
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
            //font.bold: true
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
        x: Math.max(parent.width/2 - width/2, userNameItem.x + userNameItem.width + 30)
        width: Math.max(40, Math.min(topBar.width > 1024 ? 400 : 250, sessionButtons.x - (userNameItem.x + userNameItem.width) - 30*2))
        height: largeVariant ? 35 : 30

        visible: tabListView.currentIndex !== 3

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 2

        Image {
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -2
            anchors.right: parent.right
            anchors.rightMargin: 11
            source: "image://generalicon/asset/search_pic.png"
        }

        BorderImage {
            border.left: 6
            border.right: 6
            border.top: 6
            border.bottom: 6
            anchors.fill: parent
            source: "image://generalicon/asset/search_bar_bg.png"
        }

        Item {
            id: searchInputFilter

            Keys.onPressed: {
                if (event.key == Qt.Key_Left ||
                        event.key == Qt.Key_Right ||
                        event.key == Qt.Key_Up ||
                        event.key == Qt.Key_Down ||
                        event.key == Qt.Key_Return ||
                        event.key == Qt.Key_Enter ||
                        event.key == Qt.Key_Tab)
                {
                    event.accepted = true
                    tabListView.processKeyboard(event.key)
                }
                else if (!searchInput.text.length) {
                    if (event.key == Qt.Key_Space) {
                        event.accepted = true
                        tabListView.currentTabIndexChanged((tabListView.currentIndex + tabListView.count) % tabListView.count + 1)
                    }
                    else if (event.key == Qt.Key_Backspace) {
                        event.accepted = true
                        tabListView.processKeyboard(event.key)
                    }
                }
            }
        }

        TextInput {
            id: searchInput
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -2
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width - 20

            selectByMouse: true
            activeFocusOnPress: false
            cursorVisible: true
            font.family: "Bitstream Vera Sans"
            color: "white"
            //font.italic: true
            font.pixelSize: largeVariant ? 16 : 14
        }
        Binding {
            target: searchGridModel
            property: "searchQuery"
            value: searchInput.text
        }
    }

    Image {
        id: sessionButtons
        sourceSize.width: largeVariant ? 114 : 85
        sourceSize.height: largeVariant ? 68 : 51
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.topMargin: largeVariant ? 10 : 5
        anchors.top: parent.top
        source: "image://generalicon/asset/session_buttons_bg.png"

        SessionButton {
            width: largeVariant ? 41 : 30
            height: largeVariant ? 41 : 30
            anchors.left: parent.left
            anchors.leftMargin: largeVariant ? 7 : 5.5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: largeVariant ? 9 : 7

            imgNormal: "image://generalicon/asset/lock.png"
            imgHover: "image://generalicon/asset/lock_hover.png"
            imgPressed: "image://generalicon/asset/lock_press.png"
            function onButtonClicked() {
                sessionProvider.lock()
            }
        }

        SessionButton {
            width: largeVariant ? 55 : 41
            height: largeVariant ? 55 : 41
            anchors.bottom: parent.bottom
            anchors.bottomMargin: largeVariant ? 6 : 5
            anchors.right: parent.right
            anchors.rightMargin: largeVariant ? 7 : 5

            imgNormal: "image://generalicon/asset/shutdown.png"
            imgHover: "image://generalicon/asset/shutdown_hover.png"
            imgPressed: "image://generalicon/asset/shutdown_press.png"
            function onButtonClicked() {
                sessionProvider.shutdown()
            }
        }
    }

    Image {
        source: "image://generalicon/asset/lines_up.png"
        anchors.top: parent.bottom
        anchors.topMargin: -1
        width: parent.width
        height: 8
    }
}
