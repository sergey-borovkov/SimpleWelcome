import QtQuick 1.0

GridView {
    id: grid
    property variant dataSource
    property variant prevGridGroup
    property variant nextGridGroup
    property int maxCount: -1
    property int indexStartAt: 0
    property bool draggable: false
    property bool enabledSystemDnD: false  // set true to enable system Drag&Drop
    property bool stackable: false // set true to enable icons stacking while Drag&Dropping
    property bool isPopupGroup: false

    property int dndSrcId: gridMouseArea.dndSrcId
    property int dragOutTopMargin: -1
    property int dragOutBottomMargin: -1

    signal selectionChangedByKeyboard(variant newCurrentItem)
    signal itemStackingChanged()

    // constants
    property int columns: constants.gridColumns
    property int cellHorizontalSpacing: Math.max(0, (parent.width - constants.cellWidth * columns) / (columns + 1))

    anchors {
        left: parent.left
        leftMargin: cellHorizontalSpacing
    }
    cellWidth: (width - cellHorizontalSpacing) / columns - 1
    cellHeight: constants.cellHeight

    delegate: Cell {}

    ListModel {
        id: appsModel
        signal itemClicked(int newIndex)
    }

    model: appsModel

    function resetContent() {
        if(typeof model !== 'undefined')
            model.clear()
    }

    function onItemClicked(newIndex) {
        dataSource.itemClicked(newIndex == -1 ? newIndex : model.get(newIndex).id)
    }

    function getCellIndex(inX, inY) {
        var curRow = Math.round((inY - (cellHeight/2)) / cellHeight)
        var curColumn = Math.max(0, Math.min(columns - 1, Math.round((inX - (constants.cellWidth/2)) / cellWidth)))
        var curIndex = Math.max(0, Math.min(count - 1, curRow * columns + curColumn))
        return curIndex
    }

    function stackItemInItem(indexStackingTo, indexDragging, notSaveChanges) {
        var itemDragging = model.get(indexDragging)
        var itemStackingTo = model.get(indexStackingTo)

        var stackArray = itemStackingTo.stack
        if (stackArray === undefined) {
            //console.log("FIRST TIME STACKING")
            stackArray = []
            stackArray.push(root.cloneObject(itemStackingTo))
        }
        else {
            //console.log("STACKING AGAIN")
            for (var i in stackArray) // Checking if item is already present in stack
                if (stackArray[i].id === itemDragging.id)
                {
                    //console.log("Duplicate ignored")
                    return false
                }
        }
        stackArray.push(root.cloneObject(itemDragging))

        //console.log("----------------- STACKING [" + indexDragging + "] " + model.get(indexDragging).caption + " to [" + indexStackingTo + "] " + model.get(indexStackingTo).caption)


        if (stackArray.length === 2) // First time stacking
            model.setProperty(indexStackingTo, "caption", itemStackingTo.caption + " Group")

        model.setProperty(indexStackingTo, "imagePath", "image://generalicon/stacked/" + itemStackingTo.imagePath.slice(28) + "|" + itemDragging.imagePath.slice(28))
        model.setProperty(indexStackingTo, "stack", stackArray)

        if (notSaveChanges !== true)
            itemStackingChanged()
        return true
    }

    function unstackItemInItem(indexUnstackingFrom, indexDragging) {
        //console.log("----------------- UNSTACKING " + indexDragging + " from " + indexUnstackingFrom)

        var itemDragging = model.get(indexDragging)
        //console.log(gridMouseArea.dndDest + " with " + itemDragging)
        var itemUnstackingFrom = model.get(indexUnstackingFrom)
        //console.log("itemUnstackingFrom: " + itemUnstackingFrom)

        var stackArray = itemUnstackingFrom.stack
        if (stackArray === undefined)
            return false

        var stackIcon = "image://generalicon/stacked/"

        var removeIndex = -1
        for (var i in stackArray) // If desired item occured - remove it. Otherwise recalculate stack icon
        {
            if (stackArray[i].id === itemDragging.id)
            {
                //console.log("Removing " +  stackArray[i].id + " - " + stackArray[i].caption + ", its == " + itemDragging.id + " - " + itemDragging.caption)
                removeIndex = i
            }
            else
            {
                //console.log("Skipping " + stackArray[i].id + " - " + stackArray[i].caption)
                stackIcon += stackArray[i].imagePath.slice(28) + "|"
            }
        }
        if (removeIndex != -1)
            stackArray.splice(removeIndex, 1)
        stackIcon = stackIcon.substring(0, stackIcon.length - 1)

        //console.log("REAL ICON PATH:" + stackIcon + " | items left: " + stackArray.length)

        if (stackArray.length === 1) {
            //console.log("LAST ELEMENT LEFT")
            var innerItem = stackArray[0]
            model.setProperty(indexUnstackingFrom, "stack", undefined)
            model.set(indexUnstackingFrom, innerItem)
        }
        else {
            //console.log("WAS: " + itemUnstackingFrom.imagePath)
            //console.log("TRUNCATING: " + itemDragging.imagePath)
            //console.log("NOEW: " + itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "imagePath", stackIcon)//itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "stack", stackArray)
        }

        // Fixing stacked item ID to match it's first element
        if (removeIndex == 0)
        {
            console.log("Fixing stacked item ID: indexUnstackingFrom: " + indexUnstackingFrom + " to " + stackArray[0].id)
            model.setProperty(indexUnstackingFrom, "id", stackArray[0].id)
            model.setProperty(indexUnstackingFrom, "caption", stackArray[0].caption)
        }
        itemStackingChanged()
        return true
    }

    Component.onCompleted: {
        //console.log("COMPLETED " + dataSource + " VIEW")
        if (dataSource)
        {
            dataSource.resetContent.connect(resetContent)
            if (dataSource.updateItemData !== undefined)
                dataSource.updateItemData.connect(updateItemContent)
        }
        model.itemClicked.connect(onItemClicked)
    }

    function updateItemContent(id, field, data) {
        for (var i = 0; i < model.count; i++)
            if (model.get(i).id === id) {
                model.setProperty(i, field, data)
                break
            }
    }

    function selectOtherGrid(gridWorkingWith, newCurrentIndex) {

        gridWorkingWith.highlightItem.moveDuration = 0
        gridWorkingWith.currentIndex = newCurrentIndex
        gridWorkingWith.forceMyFocus()
        gridWorkingWith.highlightItem.moveDuration = gridWorkingWith.highlightItem.moveDurationConst
        return gridWorkingWith.currentItem
    }

    function processKeyboard(key) {
        var newCurrentItem
        switch (key)
        {
        case Qt.Key_Left:
            if (currentIndex == 0 && prevGridGroup)
                newCurrentItem = selectOtherGrid(prevGridGroup.gridView, prevGridGroup.count - 1)

            if (!interactive)
                moveCurrentIndexLeft()
            break
        case Qt.Key_Right:
            if (currentIndex == count - 1 && nextGridGroup)
                newCurrentItem = selectOtherGrid(nextGridGroup.gridView, 0)

            if (!interactive)
                moveCurrentIndexRight()
            break
        case Qt.Key_Up:
            if (currentIndex < columns && prevGridGroup)
            {
                var roundCount = Math.floor((prevGridGroup.count) / columns) * columns
                var newCur = (currentIndex % columns) + roundCount - columns * Math.min(1, Math.floor((currentIndex % columns) / (prevGridGroup.count - roundCount)))

                newCurrentItem = selectOtherGrid(prevGridGroup.gridView, newCur)
            }

            if (!interactive)
                moveCurrentIndexUp()
            break
        case Qt.Key_Down:
            if (currentIndex >= count - columns && nextGridGroup)
            {
                newCurrentItem = selectOtherGrid(nextGridGroup.gridView, currentIndex % columns)
            }

            if (!interactive)
                moveCurrentIndexDown()
            break

        case Qt.Key_Return:
            model.itemClicked(currentIndex)

            break
        case Qt.Key_Enter:
            model.itemClicked(currentIndex)
            break

        case Qt.Key_Backspace:
            //model.itemClicked(-1)
            if (isPopupGroup)
                gridsListView.hideGroup()
            break

        case Qt.Key_Tab:
            console.log("KEY TAB")
            if (isPopupGroup) {
                groupLabel.forceActiveFocus()
            }
            break
        }

        if (key == Qt.Key_Left || key == Qt.Key_Right ||
            key == Qt.Key_Up || key == Qt.Key_Down)
            selectionChangedByKeyboard(newCurrentItem == null ? currentItem : newCurrentItem)
    }

    Keys.onPressed: {
        processKeyboard(event.key)
    }


    states: State {
        name: "ShowBars"
        when: grid.movingVertically
        PropertyChanges { target: verticalScrollBar; opacity: 1 }
    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }

    ScrollBar {
        id: verticalScrollBar
        width: 12;
        height: grid.height - 12

        anchors.right: grid.right
        opacity: 0
        orientation: Qt.Vertical
        position: grid.visibleArea.yPosition
        pageSize: grid.visibleArea.heightRatio
    }
}

