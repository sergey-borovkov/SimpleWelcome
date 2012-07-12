import QtQuick 1.1

FocusScope {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    property variant grid: searchGridView

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

                GroupText {
                    text: "Command Line"
                    visible: searchGridView.count != 0
                }

                AppsGridView {
                    id: searchGridView
                    model: searchGridModel
                    width: parent.width
                    height: Math.ceil(count / columns) * 200
                    property string groupName: "Command Line"

                    prevGrid: documentsGridView
                    nextGrid: favoritesGridView

                    interactive: false
                    focus: true

                    Connections {
                        target: searchGridView
                        onAdd: {
                            console.log("ADDED!!!!!")
                        }
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

                GroupText {
                    text: "Recent Documents"
                    visible: favoritesGridView.count != 0
                }

                AppsGridView {
                    id: favoritesGridView
                    model: favoritesGridModel
                    width: parent.width
                    height: Math.ceil(count / columns) * 200
                    property string groupName: "Recent Documents"

                    prevGrid: searchGridView
                    nextGrid: documentsGridView

                    interactive: false

                    Component.onCompleted: {
                        highlightItem.opacity = 0
                    }

                }
            }

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height

                GroupText {
                    text: "Applications"
                    visible: documentsGridView.count != 0
                }

                AppsGridView {
                    id: documentsGridView

                    model: documentsGridModel
                    width: parent.width
                    height: Math.ceil(count / columns) * 200
                    property string groupName: "Applications"
                    interactive: false

                    prevGrid: favoritesGridView
                    nextGrid: recentAppsGridView

                    Component.onCompleted: {
                        highlightItem.opacity = 0
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
