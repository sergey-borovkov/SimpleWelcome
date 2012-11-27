import QtQuick 1.1

FocusScope {
    id: popupFrame
    width: parent.width
    z: 1

    property int slideHeight: 0
    property int stackedIconIndex: -1
    property alias gridGroup: appsGrid
    property alias arrowX: arrow.x
    property alias groupTitle: appsGrid.groupName

    clip: true
    state: "CLOSED"

    function groupCloseCompleted() {
        if (gridMouseArea.dndSrcId === -1)
            gridsListView.dndStateChanged(false)
    }

    Item {
        id: popupGroupWrapper
        width: parent.width
        height: childrenRect.height + 3

        Image {
            id: arrow
            anchors {
                bottom: groupBorder.top
                bottomMargin: -1
            }
            z: 1

            source: "image://generalicon/asset/popup_arrow.png"
        }

        Rectangle {
            id: groupBorder
            anchors {
                top: parent.top
                topMargin: 14
            }
            x: -1
            width: parent.width + 1
            height: childrenRect.height + 64

            color: "#505050"
            focus: true
            border.color: "#A0A0A0"

            GridWithGroup {
                id: appsGrid
                anchors {
                    top: parent.top
                    topMargin: 32
                }
                width: parent.width
                height: childrenRect.height

                isPopupGroup: true
                draggable: true
                dragOutTopMargin: 72
                dragOutBottomMargin: 33
                mouseDragChangesGrids: true
                groupCellHeight: (tabRoot.height - 75 - tabRoot.height / 5 - 10) / 4 - 1
                dataSource: dataSource_Apps
            }
        }

        Image {
            source: "image://generalicon/asset/popup_group_shadow.png"
            anchors.top: groupBorder.bottom
            anchors.topMargin: 1
            width: groupBorder.width
            z: 1
        }
    }

    states: [
        State {
            name: "CLOSED"
            PropertyChanges {
                target: popupFrame
                height: 0
                slideHeight: 0
            }
        },

        State {
            name: "OPEN"
            PropertyChanges {
                target: popupFrame
                height: childrenRect.height
                slideHeight: y + height < tabRoot.height ? 0 : -(tabRoot.height - (y + height - 6))
            }
        }
    ]

    transitions: [
        Transition {
            from: "CLOSED"
            to: "OPEN"

            NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "OPEN"
            to: "CLOSED"

            SequentialAnimation {
                NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
                ScriptAction { script: popupFrame.groupCloseCompleted() }
            }
        }
    ]

    Component.onCompleted: {
        gridGroup.groupNameChanged.connect(gridsListView.groupNameChanged)
    }
}
