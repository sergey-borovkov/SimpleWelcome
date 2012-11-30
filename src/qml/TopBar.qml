import QtQuick 1.1

FocusScope {
    id: topBar
    anchors.top: parent.top
    width: parent.width
    height: largeVariant ? 80 : 60

    property bool largeVariant: root.height > 800

    property alias searchText: searchInput.text

    focus: true
    Keys.forwardTo: [searchInputFilter, searchInput]

    Image {
        id: userIcon
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 16
        }
        sourceSize {
            width: topBar.height - 16
            height: topBar.height - 16
        }

        source: "image://generalicon/general/usericon"
    }

    Text {
        id: usernameLabel
        anchors {
            left: userIcon.right
            leftMargin: 12
            right: searchField.left
            rightMargin: 12
            verticalCenter: parent.verticalCenter
        }

        maximumLineCount: 1
        elide: Text.ElideRight
        wrapMode: Text.Wrap
        font {
            pointSize: 18
            family: "Bitstream Vera Sans"
        }
        style: Text.Raised
        styleColor: "#000"
        color: "#eee"

        Component.onCompleted: {
            text = userInfoProvider.userName
        }
    }

    Item {
        id: searchField
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 2
        }
        width: topBar.width > 1024 ? 400 : topBar.width > 800 ? 250 : 200
        height: largeVariant ? 35 : 30

        visible: tabListView.currentIndex !== 3

        Image {
            id: magnifyingGlassPic
            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -2
                right: parent.right
                rightMargin: 11
            }

            source: "image://generalicon/asset/search_pic.png"
        }

        BorderImage {
            id: searchBarBackground
            border {
                left: 6
                right: 6
                top: 6
                bottom: 6
            }
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
            color: "white"
            font {
                family: "Bitstream Vera Sans"
                pixelSize: largeVariant ? 16 : 14
            }
        }

        Binding {
            target: searchGridModel
            property: "searchQuery"
            value: searchInput.text
        }
    }

    Image {
        id: sessionButtons
        anchors {
            right: parent.right
            rightMargin: 10
            topMargin: largeVariant ? 10 : 5
            top: parent.top
        }
        sourceSize {
            width: largeVariant ? 114 : 85
            height: largeVariant ? 68 : 51
        }

        source: "image://generalicon/asset/session_buttons_bg.png"

        SessionButton {
            id: lockButton
            anchors {
                left: parent.left
                leftMargin: largeVariant ? 7 : 5.5
                bottom: parent.bottom
                bottomMargin: largeVariant ? 9 : 7
            }
            width: largeVariant ? 41 : 30
            height: largeVariant ? 41 : 30

            imgNormal: "image://generalicon/asset/lock.png"
            imgHover: "image://generalicon/asset/lock_hover.png"
            imgPressed: "image://generalicon/asset/lock_press.png"

            function onButtonClicked() {
                sessionProvider.lock()
            }
        }

        SessionButton {
            id: shutdownButton
            anchors {
                bottom: parent.bottom
                bottomMargin: largeVariant ? 6 : 5
                right: parent.right
                rightMargin: largeVariant ? 7 : 5
            }
            width: largeVariant ? 55 : 41
            height: largeVariant ? 55 : 41

            imgNormal: "image://generalicon/asset/shutdown.png"
            imgHover: "image://generalicon/asset/shutdown_hover.png"
            imgPressed: "image://generalicon/asset/shutdown_press.png"

            function onButtonClicked() {
                sessionProvider.shutdown()
            }
        }
    }

    Image {
        id: linesDecoration
        anchors {
            top: parent.bottom
            topMargin: -1
        }
        width: parent.width
        height: 8

        source: "image://generalicon/asset/lines_up.png"
    }
}
