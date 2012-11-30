import QtQuick 1.1

Column {
    id: groupRoot

    width: parent.width
    height: childrenRect.height

    signal gridCurrentItemChanged(variant newCurrentItem)
    signal showPopupGroup(int index, variant stackItemData, variant iconCoords)
    signal gridMyFocusChanged(int containerIndex)
    signal groupNameChanged(string newName)

    // constants
    property int groupCellHeight: constants.cellHeight
    property int textToGridSpacing: constants.textToGridSpacing
    property int textHeight: constants.groupTextHeight
    property int containerIndex: 0

    // Dict-defined properties
    property alias groupName: groupLabel.text
    property alias dataSource: iconGridView.dataSource
    property alias draggable: iconGridView.draggable
    property alias enabledSystemDnD: iconGridView.enabledSystemDnD
    property alias groupNameVisible: groupLabelWrapper.visible
    property alias stackable: iconGridView.stackable
    property alias mouseDragChangesGrids: iconGridView.mouseDragChangesGrids
    // iconGridView aliases
    property alias prevGridGroup: iconGridView.prevGridGroup
    property alias nextGridGroup: iconGridView.nextGridGroup
    property alias maxCount: iconGridView.maxCount
    property alias indexStartAt: iconGridView.indexStartAt
    property alias count: iconGridView.count
    property alias gridView: iconGridView
    property alias dragOutTopMargin: iconGridView.dragOutTopMargin
    property alias dragOutBottomMargin: iconGridView.dragOutBottomMargin
    property alias myActiveFocus: iconGridView.myActiveFocus
    property alias isPopupGroup: iconGridView.isPopupGroup

    spacing: textToGridSpacing
    //visible: iconGridView.count != 0

    Item {
        id: groupLabelWrapper
        anchors.left: parent.left
        width: parent.width
        height: groupLabel.text || isPopupGroup ? textHeight : 0

        TextInput {
            id: groupLabel
            anchors {
                left: parent.left
                leftMargin: 39
            }
            height: parent.height

            readOnly: !isPopupGroup
            activeFocusOnPress: isPopupGroup
            font {
                family: "Bitstream Vera Sans"
                bold: true
                pixelSize: 14//18
            }
            color: "#eee"

            onTextChanged: {
                if (isPopupGroup)
                    groupNameChanged(groupName)
            }

            onActiveFocusChanged: {
                if (activeFocus)
                    iconGridView.myActiveFocus = false
            }

            Keys.onEnterPressed: {
                if (isPopupGroup)
                    gridsListView.hideGroup()
            }

            Keys.onReturnPressed: {
                if (isPopupGroup)
                    gridsListView.hideGroup()
            }
            Keys.onTabPressed: {
                iconGridView.myActiveFocus = true
                event.accepted = false
            }

            BorderImage {
                id: textBackground
                anchors {
                    left: parent.left
                    leftMargin: -10
                    top: parent.top
                    topMargin: -7
                }
                border {
                    left: 6
                    right: 6
                    top: 6
                    bottom: 6
                }
                width: groupLabel.width + 20
                height: 30
                z: -1

                visible: !groupLabel.readOnly
                source: "image://generalicon/asset/search_bar_bg.png"
            }
        }

        Image {
            id: lineDecoration
            anchors {
                left: parent.left
                leftMargin: 3
                bottom: parent.bottom
                bottomMargin: -2
            }
            width: parent.width - 3 - 42 - 22
            height: 3
            z: 1

            source: "image://generalicon/asset/group_line.png"
        }
    }

    IconGridView {
        id: iconGridView
    }

    Component {
        id: highlightComponent

        Item {
            id: gridSelection
            x: iconGridView.currentItem ? iconGridView.currentItem.x : 0
            y: iconGridView.currentItem ? iconGridView.currentItem.y : 0
            width: iconGridView.currentItem ? iconGridView.currentItem.width : 0
            height: iconGridView.currentItem ? iconGridView.currentItem.height : 0

            property int animationDuration: 150
            property int moveDurationConst: 150
            property int moveDuration: moveDurationConst

            opacity: myActiveFocus ? 1 : 0

            BorderImage {
                id: gridSelectionPic
                anchors {
                    fill: parent
                    rightMargin: -2
                    bottomMargin: -2
                }
                border {
                    left: 5
                    right: 7
                    top: 5
                    bottom: 7
                }

                source: "image://generalicon/asset/grid_selection.png"
            }

            Behavior on x {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on y {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on width {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on height {
                NumberAnimation { duration: moveDuration }
            }

            Behavior on opacity {
                NumberAnimation { duration: animationDuration }
            }
        }
    }

    states: [
        State {
            name: "clipped"
            PropertyChanges {
                target: groupRoot
                clip: true
            }
        },

        State {
            name: "unclipped"
            PropertyChanges {
                target: groupRoot
                clip: false
            }
        }
    ]

    transitions: Transition {
        from: "clipped"
        to: "unclipped"
        PropertyAnimation { target: groupRoot; properties: "clip"; duration: 300 }
    }

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
        iconGridView.myActiveFocusChanged.connect(gridMyFocusChanged)
    }
} // groupRoot
