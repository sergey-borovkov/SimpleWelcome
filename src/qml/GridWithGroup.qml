import QtQuick 1.1

Column {
    id: groupRoot
    // Dict-defined properties
    property alias groupName: groupLabel.text
    property alias dataSource: iconGridView.dataSource
    property alias draggable: iconGridView.draggable
    property alias enabledSystemDnD: iconGridView.enabledSystemDnD
    property alias groupNameVisible: groupLabel.visible
    property alias stackable: iconGridView.stackable

    property alias prevGridGroup: iconGridView.prevGridGroup
    property alias nextGridGroup: iconGridView.nextGridGroup
    property alias maxCount: iconGridView.maxCount
    property int groupCountStart: 0

    property alias count: iconGridView.count
    property alias gridView: iconGridView

    property alias dragOutTopMargin: iconGridView.dragOutTopMargin
    property alias dragOutBottomMargin: iconGridView.dragOutBottomMargin
    property alias myActiveFocus: iconGridView.myActiveFocus

    property bool isPopupGroup: false
    property int groupCellHeight: constants.cellHeight

    signal gridCurrentItemChanged(variant newCurrentItem)
    signal showPopupGroup(int index, variant stackItemData, variant iconCoords)
    signal gridMyFocusChanged(int containerIndex)
    signal groupNameChanged(string newName)

    property int containerIndex: 0

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


    // constants
    property int textToGridSpacing: constants.textToGridSpacing
    property int textHeight: constants.groupTextHeight

    spacing: textToGridSpacing
    width: parent.width
    height: childrenRect.height
    //visible: iconGridView.count != 0

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
        iconGridView.myActiveFocusChanged.connect(gridMyFocusChanged)
    }

    TextInput {
        id: groupLabel
        width: parent.width
        height: groupLabel.text || isPopupGroup ? textHeight : 0
        anchors.left: parent.left
        anchors.leftMargin: 39//16
        readOnly: !isPopupGroup
        activeFocusOnPress: isPopupGroup

        font.family: "Bitstream Vera Sans"
        font.bold: true
        font.pixelSize: 14//18
        color: "#eee"
        //styleColor: "#000"

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

        Image {
            source: "image://generalicon/asset/group_line.png"
            width: parent.width - 3 - 42 - 22
            height: 3
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -2
            z: 1
        }
    }

    Component {
        id: highlightComponent
        Item {
            id: gridSelection
            property int animationDuration: 150
            property int moveDurationConst: 150
            property int moveDuration: moveDurationConst
            opacity: myActiveFocus ? 1 : 0

            width: iconGridView.currentIndex !== -1 ? iconGridView.currentItem.width : 0
            height: iconGridView.currentIndex !== -1 ? iconGridView.currentItem.height : 0
            x: iconGridView.currentIndex !== -1 ? iconGridView.currentItem.x : 0
            y: iconGridView.currentIndex !== -1 ? iconGridView.currentItem.y : 0

            BorderImage {
                border.left: 5
                border.right: 7
                border.top: 5
                border.bottom: 7

                anchors.fill: parent
                anchors.rightMargin: -2
                anchors.bottomMargin: -2

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

    IconGridView {
        id: iconGridView

        width: parent.width
        height: groupName == "Apps" ? gridsListView.height : Math.ceil(count / columns) * groupCellHeight
        cellHeight: groupCellHeight
        interactive: false

        highlight: highlightComponent
        highlightFollowsCurrentItem: false

        property bool myActiveFocus: false
        signal myActiveFocusChanged(int containerIndex)

        function newItemData(itemData)
        {
            // This is needed for delegate to not blaming unknown variable
            if (itemData.pinned === undefined)
                itemData.pinned = undefined

            if (itemData.stack === undefined)
                itemData.stack = undefined
            model.append(itemData)
        }

        function newItemDataAt(pos, itemData) {
            // This is needed for delegate to not blaming unknown variable
            if (itemData.pinned === undefined)
                itemData.pinned = undefined

            if (itemData.stack === undefined)
                itemData.stack = undefined
            model.insert(pos, itemData)
        }

        function onItemClicked(newIndex)
        {
            if (newIndex != -1)
            {
                var realIndex = model.get(newIndex).id
                if (model.get(newIndex).stack !== undefined)
                {
                    //console.log("onItemClicked::showPopupGroup from: " + newIndex)
                    var iconCoords = mapToItem(groupTab, currentItem.x + currentItem.width / 2 - 8, currentItem.y + currentItem.height)
                    showPopupGroup(newIndex, model.get(newIndex), iconCoords)
                    gridView.myActiveFocus = false
                    return
                }
                if (groupName == i18n_Recent_Applications || groupName == i18n_Recent_Documents)
                {
                    dndSrcId = realIndex
                    model.move(newIndex, 0, 1)

                    for (var i = 0; i < model.count; i++)
                        model.setProperty(i, "id", i)

                    dndSrcId = -1
                    gridView.currentIndex = 0
                }
            }
            if (newIndex == -1)
                dataSource.itemClicked(-1)
            else
                dataSource.itemClicked(realIndex, groupName)
        }

        function forceMyFocus() {
            myActiveFocus = true
            myActiveFocusChanged(containerIndex)
            //console.log("myActiveFocusChanged to " + containerIndex)
        }

        Component.onCompleted: {
            if ('group' in model)
                model.group = groupName
        }
    }
}
