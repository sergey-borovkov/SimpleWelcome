import QtQuick 1.1

Rectangle {
    id: tabBar

    anchors.bottom: parent.bottom
    width: parent.width
    height: 80
    //color: "grey"
    color: "transparent"

    Row {
        //anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        spacing: 40

        TabButton {
            label: "First Tab"
            id: firstTabButton
            onButtonClick: { tabListView.currentIndex = 1; page.reloadTabs(); }
        }

        TabButton {
            label: "Second Tab"
            id: secondTabButton
            onButtonClick: { tabListView.currentIndex = 2; page.reloadTabs(); }
        }
    }
}
