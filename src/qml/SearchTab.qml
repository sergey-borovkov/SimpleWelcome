import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: searchCmdGridView

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: gridsContainer.height + 32
        boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: gridsContainer
            width: parent.width
            height: childrenRect.height
            anchors.bottom: parent.bottom
            spacing: 32

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height
                visible: searchCmdGridView.count != 0

                GroupText {
                    text: "Command Line"
                }

                AppsGridView {
                    id: searchCmdGridView
                    model: searchCmdGridModel

                    width: parent.width
                    height: Math.ceil(count / columns) * 200

                    prevGrid: searchAppsGridView
                    nextGrid: searchRecentDocsGridView

                    interactive: false
                    focus: true

                    Component.onCompleted: {
                        model.group = "Command Line"
                    }

                    Behavior on height {
                        NumberAnimation { duration: 150 }
                    }
                }
            }

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height
                visible: searchRecentDocsGridView.count != 0

                GroupText {
                    text: "Recent Documents"
                }

                AppsGridView {
                    id: searchRecentDocsGridView
                    model: searchRecentDocsGridModel

                    width: parent.width
                    height: Math.ceil(count / columns) * 200

                    prevGrid: searchCmdGridView
                    nextGrid: searchAppsGridView

                    interactive: false

                    GridView.onRemove: {
                        console.log("ON ADD")
                        highlightItem.opacity = 0
                    }

                    Component.onCompleted: {
                        model.group = "Recent Documents"
                    }

                }
            }

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height
                visible: searchAppsGridView.count != 0

                GroupText {
                    text: "Applications"
                }

                AppsGridView {
                    id: searchAppsGridView
                    model: searchAppsGridModel

                    width: parent.width
                    height: Math.ceil(count / columns) * 200

                    prevGrid: searchRecentDocsGridView
                    nextGrid: searchCmdGridView

                    interactive: false

                    Component.onCompleted: {
                        model.group = "Applications"
                    }
                }
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
