import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width

    property variant grid: groupTab.tabListView.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        property int tabCellHeight: (tabListView.height - (constants.textToGridSpacing + constants.groupTextHeight)*3 - constants.gridWithGroupsSpacing*2) / 4
        groups: [
            {
                groupName: i18n_Recent_Applications,
                dataSource: dataSource_RecentApps,
                draggable: true,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight
            },
            {
                groupName: i18n_Favorites,
                dataSource: dataSource_Favorites,
                //draggable: true,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight
            },
            {
                groupName: i18n_Recent_Documents,
                dataSource: dataSource_Documents,
                enabledSystemDnD: true,
                groupCellHeight: tabCellHeight
            },
        ]

        isForceOnOneScreen: true
    }
}
