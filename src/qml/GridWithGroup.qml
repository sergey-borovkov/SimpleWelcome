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
    property alias startIndex: iconGridView.startIndex
    property alias endIndex: iconGridView.endIndex

    property alias count: iconGridView.count
    property alias gridView: iconGridView

    property alias dragOutTopMargin: iconGridView.dragOutTopMargin
    property alias dragOutBottomMargin: iconGridView.dragOutBottomMargin

    signal gridItemCountChanged
    signal gridCurrentItemChanged(variant newCurrentItem)
    signal showPopupGroup(int index, variant stackItemData, variant iconCoords)

    // constants
    property int textToGridSpacing: constants.textToGridSpacing
    property int textHeight: constants.groupTextHeight

    spacing: textToGridSpacing
    width: parent.width
    height: childrenRect.height
    //visible: iconGridView.count != 0

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
    }

    Text {
        id: groupLabel
        width: parent.width
        height: groupLabel.text ? textHeight : 0
        maximumLineCount: 1
        anchors.left: parent.left
        anchors.leftMargin: 39//16

        font.family: "Bitstream Vera Sans"
        font.bold: true
        font.pixelSize: 14//18
        color: "#eee"
        styleColor: "#000"

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
        height: Math.ceil(count / columns) * gridCellHeight

        interactive: false

        function updateSelection()
        {
            //parent.count = count // without this it is updated too late// TEST WHAT THE HELL NOW'S HAPPENING
            if (highlightItem && (updateSelection.countWas === undefined || updateSelection.countWas < count))
            {
                highlightItem.animationDuration = 0
                highlightItem.opacity = 0
                highlightItem.animationDuration = 150
            }
            gridItemCountChanged()
            updateSelection.countWas = count
        }

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

        function newItemData(iconPath, name, itemId, advParam)
        {
            if (startIndex === endIndex && endIndex === -1)
            {
                for (var i = 0; i < model.count; i++)
                    if (model.get(i).id === itemId)
                        return
            }
            else if (!(startIndex <= itemId && itemId <= endIndex &&
                       count <= endIndex - startIndex)) // Last condition eliminates duplicates via limiting item count. Not the best solution, fix someday
                return


            if (typeof advParam == 'boolean') // New item from RecentApps, advParam is pinned state of icon
            {
                appendItemToModel({ imagePath: iconPath, caption: name, id: itemId, pinned: advParam })
            }
            else if (typeof advParam == 'string') // New item from search, advParam is group
            {
                if (advParam == groupName)
                {
                    //console.log("Added " + name + " : " + itemId + " in [" + startIndex + "-" + endIndex + "] of " + count)
                    appendItemToModel({ imagePath: iconPath, caption: name, id: itemId, pinned: undefined, group: groupName })
                }
            }
            else
                appendItemToModel({ imagePath: iconPath, caption: name, id: itemId, pinned: undefined })
            //console.log("--- Added [" + startIndex + " to " + endIndex + "] with id: " + itemId)
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
                    return
                }
                if (groupName == i18n_Recent_Applications || groupName == i18n_Recent_Documents)
                {
                    dndSrcId = realIndex
                    model.move(newIndex, 0, 1)

                    for (var i = 0; i < model.count; i++)
                        if (model.get(i).id < realIndex)
                            model.set(i, {"id": model.get(i).id + 1})
                        else if (model.get(i).id == realIndex)
                            model.set(i, {"id": 0})

                    dndSrcId = -1
                    gridView.currentIndex = 0
                }
            }
            if (newIndex == -1)
                dataSource.itemClicked(-1)
            else
                dataSource.itemClicked(realIndex, model.get(newIndex).group)
        }


        Component.onCompleted: {
            if ('group' in model)
                model.group = groupName
            updateSelection()
        }

        onCountChanged: {
            updateSelection()
        }
    }
}
