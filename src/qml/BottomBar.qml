import QtQuick 1.1

Item {
    id: tabBar

    anchors.bottom: parent.bottom
    width: parent.width
    height: 80

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        spacing: 40

        TabButton {
            label: "Welcome"
            id: welcomeTabButton
            onButtonClick: { tabListView.currentIndex = 1; page.reloadTabs(); }
            active: tabListView.currentIndex == 1
        }

        TabButton {
            label: "Applications"
            id: applicationsTabButton
            onButtonClick: { tabListView.currentIndex = 2; page.reloadTabs(); }
            active: tabListView.currentIndex == 2
        }

        TabButton {

            label: "Timeframe"
            id: thirdTabButton
            onButtonClick: { tabListView.currentIndex = 3; page.reloadTabs();}
            active: tabListView.currentIndex == 3
        }
    }
}
