import QtQuick 1.1

Item {
    id: tabBar

    anchors.bottom: parent.bottom
    width: parent.width
    height: 80
    //color: "grey"
    //color: "transparent"

    Row {
        //anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        spacing: 40

        TabButton {
            label: "Welcome"
            id: welcomeTabButton
            onButtonClick: { tabListView.currentIndex = 1; page.reloadTabs(); }
        }

        TabButton {
            label: "Applications"
            id: applicationsTabButton
            onButtonClick: { tabListView.currentIndex = 2; page.reloadTabs(); }
        }

        TabButton {

            label: "Timeframe"
            id: thirdTabButton
            onButtonClick: { tabListView.currentIndex = 3; page.reloadTabs();}
        }
    }
}
