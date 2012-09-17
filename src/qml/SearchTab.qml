import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: groupTab.tabListView.currentItem.activeGridGroup.gridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                groupName: i18n_shell,//"Command Line",
                dataSource: searchGridModel
            },
            {
                groupName: i18n_recentdocuments, //"Recent Documents",
                dataSource: searchGridModel
            },
            {
                groupName: i18n_services, //"Applications",
                dataSource: searchGridModel
            },
        ]
    }
}
