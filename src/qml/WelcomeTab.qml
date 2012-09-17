import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: groupTab.tabListView.currentItem.activeGridGroup.gridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                groupName: i18n_Recent_Applications,
                dataSource: dataSource_RecentApps,
                draggable: true
            },
            {
                groupName: i18n_Favorites,
                dataSource: dataSource_Favorites,
                draggable: true
            },
            {
                groupName: i18n_Recent_Documents,
                dataSource: dataSource_Documents
            },
        ]

        isForceOnOneScreen: true
    }
}
