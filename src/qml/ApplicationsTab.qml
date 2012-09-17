import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width
    clip: true

    property variant grid: groupTab.tabListView.currentItem.activeGridGroup.gridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                groupName: "Ololo",
                dataSource: dataSource_Apps,
                draggable: true,
                groupNameVisible: false
            }
        ]
    }
}
