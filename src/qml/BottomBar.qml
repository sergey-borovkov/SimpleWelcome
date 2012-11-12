import QtQuick 1.1
import Private 0.1

Item {
    id: tabBar

    anchors.bottom: parent.bottom
    width: parent.width
    height: 50
    signal wheelScroll(int delta)

    // Scroll using mouse wheel
    WheelArea {
        id: wheelArea
        anchors.fill: parent

        onScrollVert: wheelScroll(delta)
    }

    Image {
        source: "image://generalicon/asset/tab_buttons_bg.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 11
        width: parent.width - 20 * 2
        height: 48

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
                label: i18n("Welcome")
                onButtonClick: { tabListView.currentIndex = 1; }
                active: tabListView.currentIndex == 1
                width: parent.width / 3
            }

            TabButton {
                id: applicationsTabButton
                label: i18n("Applications")
                onButtonClick: { tabListView.currentIndex = 2; }
                active: tabListView.currentIndex == 2
                width: parent.width / 3
            }

            TabButton {
                id: timeFrameTabButton
                label: i18n("TimeFrame")
                onButtonClick: { tabListView.currentIndex = 3; }
                active: tabListView.currentIndex == 3
                width: parent.width / 3
            }
        }

    }
}
