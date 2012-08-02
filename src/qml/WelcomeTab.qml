import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid//: gridsContainer.activeGridView

    GridWithGroupTab {
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
