import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    anchors.topMargin: 16
    property variant grid: groupTab.tabListView.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                groupName: i18n_Recent_Applications,
                dataSource: dataSource_RecentApps,
                draggable: true,
                enabledSystemDnD: true
            },
            {
                groupName: i18n_Favorites,
                dataSource: dataSource_Favorites,
                //draggable: true,
                enabledSystemDnD: true
            },
            {
                groupName: i18n_Recent_Documents,
                dataSource: dataSource_Documents,
                enabledSystemDnD: true
            },
        ]

        isForceOnOneScreen: true
    }
}
