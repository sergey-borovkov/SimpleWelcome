import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width

    property variant grid: groupTab.tabListView.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                groupName: "Apps",
                dataSource: dataSource_Apps,
                draggable: true,
                enabledSystemDnD: true,
                groupNameVisible: false,
                stackable: true,
                groupCellHeight: tabListView.height / 5 - 1
            }
        ]
    }
}
