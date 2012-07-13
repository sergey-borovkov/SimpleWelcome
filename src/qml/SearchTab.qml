import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: gridsContainer.activeGridView

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: gridsContainer.height + 32
        boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
        flickableDirection: Flickable.VerticalFlick

        GridWithGroupContainer {
            id: gridsContainer

            GridWithGroup {
                groupName: "Command Line"
                gridModel: searchCmdGridModel
                defaultFocus: true
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

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
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
