import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: gridsContainer.activeGridView

    GridWithGroupTab {
        id: flick

        GridWithGroupContainer {
            id: gridsContainer

            GridWithGroup {
                groupName: "Command Line"
                gridModel: searchCmdGridModel
            }

            GridWithGroup {
                groupName: "Recent Documents"
                gridModel: searchRecentDocsGridModel
            }

            GridWithGroup {
                groupName: "Applications"
                gridModel: searchAppsGridModel
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
