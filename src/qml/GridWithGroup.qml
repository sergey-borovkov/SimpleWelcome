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

    property alias prevGridGroup: iconGridView.prevGrid
    property alias nextGridGroup: iconGridView.nextGrid
    property alias maxCount: iconGridView.maxCount

    property alias count: iconGridView.count
    property alias gridView: iconGridView

    property alias dragOutTopMargin: iconGridView.dragOutTopMargin
    property alias dragOutBottomMargin: iconGridView.dragOutBottomMargin
    property alias myActiveFocus: iconGridView.myActiveFocus

    property bool isPopupGroup: false
    property int groupCellHeight: constants.cellHeight

    signal gridItemCountChanged
    signal gridCurrentItemChanged(variant newCurrentItem)
    signal showPopupGroup(int index, variant stackItemData, variant iconCoords)
    signal gridMyFocusChanged(int containerIndex)

    property int containerIndex: 0

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
            if (isPopupGroup && popupFrame.stackedIconIndex !== -1) {
                gridsListView.activeGridView.model.setProperty(popupFrame.stackedIconIndex, "caption", groupName)
            }
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

    IconGridView {
        id: iconGridView

        width: parent.width
        height: groupName == "Apps" ? gridsListView.height : Math.ceil(count / columns) * groupCellHeight
        cellHeight: groupCellHeight
        interactive: false

        property bool myActiveFocus: false
        signal myActiveFocusChanged(int containerIndex)

        function appendItemToModel(itemData)
        {
            itemData.stack = undefined
            model.append(itemData)

            // UNREM THIS TO ENABLE AUTO-STACKING
            /*/if (!groupNameVisible) // workaround to apply this to apps tab only
            {
                appendItemToModel.lastItem = model.get(model.count - 1)
                if (appendItemToModel.lastItem && appendItemToModel.lastLetter != appendItemToModel.lastItem.caption.charAt(0).toLowerCase())
                {
                    appendItemToModel.lastLetter = appendItemToModel.lastItem.caption.charAt(0).toLowerCase()
                    appendItemToModel.lastLetterItem = appendItemToModel.lastItem
                    //console.log("CAHNGE OF")
                }
                //if (appentItemToModel.lastItem)
                //    console.log("last item: " + appentItemToModel.lastItem.caption + "; last letter: " + appentItemToModel.lastLetter + "; stored_first: " + appentItemToModel.lastItem.caption.charAt(0).toLowerCase())

//                console.log(appentItemToModel.lastLetter)
                if (itemData.caption.charAt(0).toLowerCase() == appendItemToModel.lastLetter)
                {
                    if (typeof appendItemToModel.lastLetterItem.stack == 'undefined')
                    {
                        //console.log("Initializing group")
                        var array = []
                        array.push(itemData)
                        appendItemToModel.lastLetterItem.stack = array
                        // UNREM THIS TO GET ONE-ITEM GROUPS
                        //appentItemToModel.lastLetterItem.imagePath = "image://generalicon/stacked/" + itemData.imagePath.slice(28) + "|"
                        //console.log(model.get(2).stack + " | " + model.get(2).stack.length)
                    }
                    else
                    {
                        //console.log("Pushing " + itemData.caption)
                        //console.log("Got still: " + model.get(2).stack)
                        var array2 = appendItemToModel.lastLetterItem.stack
                        array2.push(itemData)
                        appendItemToModel.lastLetterItem.stack = array2
                        appendItemToModel.lastLetterItem.imagePath = "image://generalicon/stacked/" + appendItemToModel.lastLetterItem.imagePath.slice(28) + "|" + itemData.imagePath.slice(28)
                        //console.log(">" + appentItemToModel.lastLetterItem.imagePath)
                        //console.log("Got after: " + model.get(2).stack)
                        //model.get(2).stack = model.get(2).stack.push(itemData)
                    }
                }
            } /**/
        }

        function newItemData(itemData)
        {
            // This is needed for delegate to not blaming unknown variable
            if (itemData.pinned === undefined)
                itemData.pinned = undefined

            appendItemToModel(itemData)
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
