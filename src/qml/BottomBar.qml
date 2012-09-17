import QtQuick 1.1

Item {
    id: tabBar

    anchors.bottom: parent.bottom
    width: parent.width
    height: 80

    Item {
        width: 45
        height: 45
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 5

        Image {
            id: rosaButton
            source: "image://generalicon/asset/rosa-launcher.png"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        Image {
            id: rosaButtonHover
            source: "image://generalicon/asset/rosa-launcher-hover.png"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            smooth: true
            opacity: 0

            Behavior on opacity {
                NumberAnimation { duration: 200; }
            }
        }

        states: [
            State {
                name: "normal";
                when: !rosaButtonMouseArea.containsMouse
                PropertyChanges { target: rosaButtonHover; opacity: 0; scale: 1 }
            },

            State {
                name: "hover";
                when: rosaButtonMouseArea.containsMouse && !rosaButtonMouseArea.pressed
                PropertyChanges { target: rosaButtonHover; opacity: 1 }
            },

            State {
                name: "pressed"
                when: rosaButtonMouseArea.pressed
                PropertyChanges { target: rosaButton; scale: 0.9 }
                PropertyChanges { target: rosaButtonHover; opacity: 1; scale: 0.91 }
            }
        ]

        MouseArea {
            id: rosaButtonMouseArea
            anchors.fill: parent

            onClicked: mainWindow.close()

            hoverEnabled: true
        }
    }

    Image {
        source: "image://generalicon/asset/tab_buttons_bg.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 11
        width: parent.width - (10 + 45 + 10) * 2
        height: 48 //33

        Item {
            id: button

            width: (parent.width-4) / 3
            height: 27
            x: welcomeTabButton.active ? welcomeTabButton.x + 2 : applicationsTabButton.active ? applicationsTabButton.x : timeFrameTabButton.x
            visible: tabListView.currentIndex != 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            //z: -1

            BorderImage {
                border.left: 6
                border.right: 7
                border.top: 0
                border.bottom: 0
                anchors.fill: parent
                anchors.rightMargin: -3
                source: "image://generalicon/asset/tab_button.png"
            }

            Behavior on x {
                NumberAnimation { duration: 200; }
            }
        }

        Row {
            id: buttonsRow
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            height: 27

            TabButton {
                id: welcomeTabButton
                label: i18n_Welcome
                onButtonClick: { tabListView.currentIndex = 1; }
                active: tabListView.currentIndex == 1
                width: parent.width / 3
            }

            TabButton {
                id: applicationsTabButton
                label: i18n_Applications
                onButtonClick: { tabListView.currentIndex = 2; }
                active: tabListView.currentIndex == 2
                width: parent.width / 3
            }

            TabButton {
                id: timeFrameTabButton
                label: i18n_TimeFrame
                onButtonClick: { tabListView.currentIndex = 3; }
                active: tabListView.currentIndex == 3
                width: parent.width / 3
            }
        }

    }
}
