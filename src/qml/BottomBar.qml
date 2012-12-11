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
        id: tabButtons
        anchors {
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: -1
            bottom: parent.bottom
            bottomMargin: 11
        }
        width: parent.width - 20 * 2 + 1
        height: 49

        source: "image://generalicon/asset/tab_buttons_bg.png"

        Item {
            id: button
            anchors {
                bottom: parent.bottom
                bottomMargin: 3
            }
            x: welcomeTabButton.active ? welcomeTabButton.x + 2
                                       : applicationsTabButton.active ? applicationsTabButton.x
                                       : timeFrameTabButton.x
            width: (parent.width-4) / 3 - 1
            height: 29

            visible: tabListView.currentIndex != 0

            BorderImage {
                id: tabButtonImage
                anchors {
                    fill: parent
                    rightMargin: -3
                }
                border {
                    left: 6
                    right: 7
                    top: 0
                    bottom: 0
                }

                source: "image://generalicon/asset/tab_button.png"
            }

            Behavior on x {
                NumberAnimation { duration: 200; }
            }
        }

        Row {
            id: buttonsRow
            anchors {
                bottom: parent.bottom
                bottomMargin: 4
            }
            width: parent.width
            height: 27

            TabButton {
                id: welcomeTabButton
                width: parent.width / 3

                label: i18n("Welcome")
                active: tabListView.currentIndex == 1

                onButtonClick: { tabListView.currentIndex = 1; }
            }

            TabButton {
                id: applicationsTabButton
                width: parent.width / 3

                label: i18n("Applications")
                active: tabListView.currentIndex == 2

                onButtonClick: { tabListView.currentIndex = 2; }
            }

            TabButton {
                id: timeFrameTabButton
                width: parent.width / 3

                label: i18n("TimeFrame")
                active: tabListView.currentIndex == 3

                onButtonClick: { tabListView.currentIndex = 3; }
            }
        }

    }
}
