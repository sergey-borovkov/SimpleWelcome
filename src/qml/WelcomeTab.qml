import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid//: gridsContainer.activeGridView
    property alias tab: groupTab

    GridWithGroupTab {
        id: groupTab
        groups: [
            {
                group: "Recent Applications",
                dataSource: dataSource_RecentApps
            },
            {
                group: "Favorites",
                dataSource: dataSource_Favorites
            },
            {
                group: "Recent documents",
                dataSource: dataSource_Documents
            },
        ]
    }
}
