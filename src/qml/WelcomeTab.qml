import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: gridsContainer.activeGridView

    GridWithGroupTab {
        id: flick

        GridWithGroupContainer {
            id: gridsContainer

            GridWithGroup {
                groupName: "Recent Applications"
                gridModel: recentAppsGridModel
            }

            GridWithGroup {
                groupName: "Favorites"
                gridModel: favoritesGridModel
            }

            GridWithGroup {
                groupName: "Recent documents"
                gridModel: documentsGridModel
            }
        }

        states: State {
            name: "ShowBars"
            when: flick.movingVertically
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }
    }

    ScrollBar {
        id: verticalScrollBar
        width: 12;
        height: flick.height - 12

        anchors.right: flick.right
        opacity: 0
        orientation: Qt.Vertical
        position: flick.visibleArea.yPosition
        pageSize: flick.visibleArea.heightRatio
    }
}
