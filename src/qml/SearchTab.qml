import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid//: gridsContainer.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                group: "Command Line",
                dataSource: searchGridModel
            },
            {
                group: "Recent Documents",
                dataSource: searchGridModel
            },
            {
                group: "Applications",
                dataSource: searchGridModel
            },
        ]
    }
}
