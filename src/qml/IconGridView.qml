import QtQuick 1.0

GridView {
    id: grid
    property variant dataSource
    property variant prevGrid
    property variant nextGrid
    property int startIndex
    property int endIndex
    property bool draggable: false
    property bool enabledSystemDnD: false  // set true to enable system Drag&Drop
    property bool stackable: false // set true to enable icons stacking while Drag&Dropping

    property alias dndSrcId: gridMouseArea.dndSrcId
    property int dragOutTopMargin: -1
    property int dragOutBottomMargin: -1

    signal dndStateChanged(bool isDrag)
    signal selectionChangedByKeyboard(variant newCurrentItem)
    signal draggedOut(variant item)
    signal itemStackingChanged()

    // constants
    property int columns: constants.gridColumns
    property int highlightMoveDurationConst: 150
    property int gridCellHeight: constants.cellHeight

    property int cellHorizontalSpacing: Math.max(0, (parent.width - constants.cellWidth * columns) / (columns + 1))

    anchors {
        left: parent.left
        leftMargin: cellHorizontalSpacing
    }
    cellWidth: (width - cellHorizontalSpacing) / columns - 1
    cellHeight: gridCellHeight

    delegate: Cell {}

    highlight: Item {
        id: gridSelection
        property int animationDuration: 150
        opacity: myActiveFocus ? 1 : 0

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

        Behavior on opacity {
            NumberAnimation { duration: animationDuration }
        }
    }

    highlightMoveDuration: highlightMoveDurationConst

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
        dataSource.itemClicked(newIndex == -1 ? newIndex : appsModel.get(newIndex).id)
    }

    function getCellIndex(inX, inY) {
        var curRow = Math.round((inY - (constants.cellHeight/2)) / cellHeight)
        var curColumn = Math.max(0, Math.min(columns - 1, Math.round((inX - (constants.cellWidth/2)) / cellWidth)))
        var curIndex = Math.max(0, Math.min(count - 1, curRow * columns + curColumn))
        return curIndex
    }

    function copyObjectByValue(obj) {
        // Copying object by value
        var newObj = new Object
        for (var s in (obj)) {
            //console.log("copying " + itemStackingTo[s])
            newObj[s] = (obj)[s]
        }
        return newObj
    }

    function stackItemInItem(indexStackingTo, indexDragging) {
        //console.log("----------------- STACKING " + gridMouseArea.dndDest + " to " + indexWaitingOn)

        var itemDragging = model.get(indexDragging)
        //console.log(gridMouseArea.dndDest + " with " + itemDragging)
        var itemStackingTo = model.get(indexStackingTo)

        var stackArray = itemStackingTo.stack
        if (stackArray === undefined) {
            //console.log("FIRST TIME STACKING")
            stackArray = []
            stackArray.push(copyObjectByValue(itemStackingTo))
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
        stackArray.push(copyObjectByValue(itemDragging))

        model.setProperty(indexStackingTo, "imagePath", "image://generalicon/stacked/" + itemStackingTo.imagePath.slice(28) + "|" + itemDragging.imagePath.slice(28))
        model.setProperty(indexStackingTo, "stack", stackArray)

        itemStackingChanged()
        return true
    }

    function unstackItemInItem(indexUnstackingFrom, indexDragging) {
        //console.log("----------------- UNSTACKING " + indexDragging + " from " + indexUnstackingFrom)

        var itemDragging = model.get(indexDragging)
        //console.log(gridMouseArea.dndDest + " with " + itemDragging)
        var itemUnstackingFrom = model.get(indexUnstackingFrom)

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

    function startDragging(index) {
        if (index != -1)
        {
            gridMouseArea.dndDest = index
            gridMouseArea.dndSrc = index
            gridMouseArea.dndSrcId = model.get(gridMouseArea.dndSrc).id
            gridMouseArea.dndDestId = gridMouseArea.dndSrcId
            //console.log("dndSrc, dndSrcId, dndDest, dndDestId: " + dndSrc + " " + dndSrcId + " " + dndDest + " " + dndDestId)
            dndStateChanged(true)

//                    console.log("NOW----------------")
//                    for (var i = 0; i < model.count; i++)
//                        console.log(model.get(i).caption + " | " + model.get(i).id + " | " + i)
//                    console.log("END----------------")
        }

    }

    Component.onCompleted: {
        //console.log("COMPLETED " + dataSource + " VIEW")
        if (dataSource)
        {
            dataSource.newItemData.connect(newItemData)
            dataSource.resetContent.connect(resetContent)
            if (dataSource.updateItemData !== undefined)
                dataSource.updateItemData.connect(updateItemContent)

            dataSource.getContent()
        }
        appsModel.itemClicked.connect(onItemClicked)
    }

    function updateItemContent(id, field, data) {
        for (var i = 0; i < model.count; i++)
            if (model.get(i).id === id) {
                model.setProperty(i, field, data)
                break
            }
    }

    function selectOtherGrid(gridWorkingWith, newCurrentIndex) {
        gridWorkingWith.highlightMoveDuration = 1
        gridWorkingWith.currentIndex = newCurrentIndex
        gridWorkingWith.forceMyFocus()
        gridWorkingWith.highlightMoveDuration = highlightMoveDurationConst
        return gridWorkingWith.currentItem
    }

    function processKeyboard(key) {
        var newCurrentItem
        switch (key)
        {
        case Qt.Key_Left:
            if (currentIndex == 0 && prevGrid)
                newCurrentItem = selectOtherGrid(prevGrid, prevGrid.count - 1)

            if (!interactive)
                moveCurrentIndexLeft()
            break
        case Qt.Key_Right:
            if (currentIndex == count - 1 && nextGrid)
                newCurrentItem = selectOtherGrid(nextGrid, 0)

            if (!interactive)
                moveCurrentIndexRight()
            break
        case Qt.Key_Up:
            if (currentIndex < columns && prevGrid)
            {
                var roundCount = Math.floor((prevGrid.count) / columns) * columns
                var newCur = (currentIndex % columns) + roundCount - columns * Math.min(1, Math.floor((currentIndex % columns) / (prevGrid.count - roundCount)))

                newCurrentItem = selectOtherGrid(prevGrid, newCur)
            }

            if (!interactive)
                moveCurrentIndexUp()
            break
        case Qt.Key_Down:
            if (currentIndex >= count - columns && nextGrid)
            {
                newCurrentItem = selectOtherGrid(nextGrid, currentIndex % columns)
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
            model.itemClicked(-1)
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

    MouseArea {
        id: gridMouseArea
        anchors.fill: parent
        hoverEnabled: true

        property int dndSrcId: -1
        property int dndSrc: -1
        property int dndDest: -1
        property int dndDestId: -1
        property int pressedOnIndex
        property variant draggedItemStackedAt

        function getItemUnderCursor(isForceRecheck)
        {
            var wasCurrentIndex = grid.currentIndex
            var mouseXReal = mouseX + grid.contentX, mouseYReal = mouseY + grid.contentY
            var wasContentX = grid.contentX, wasContentY = grid.contentY
            var indexUnderMouse = grid.indexAt(mouseXReal, mouseYReal)
            var result = new Array()
            result.index = -1// = {"index": -1}

            if (indexUnderMouse != -1 && (grid.currentIndex != indexUnderMouse || isForceRecheck))
            {
                // Dirty hack to check if there is a need to change the current item after mouse position have changed
                grid.currentIndex = indexUnderMouse
                if (grid.currentItem && grid.currentItem.x < mouseXReal && grid.currentItem.y < mouseYReal &&
                      grid.currentItem.x + grid.currentItem.width > mouseXReal && grid.currentItem.y + grid.currentItem.height > mouseYReal)
                {
                    result.index = indexUnderMouse
                    result.item = grid.currentItem
                }
                grid.currentIndex = wasCurrentIndex
                grid.contentX = wasContentX
                grid.contentY = wasContentY
            }
            return result
        }

        Timer {
            id: mouseHoverTimer
            interval: stackable ? 300 : 0
            property variant itemWaitingOn: undefined
            property variant indexWaitingOn: undefined
            property bool isAimingOnStacking
            property real xWaiting
            property real yWaiting

            function calculateExpectations(mouseX, mouseY) {
                if (itemWaitingOn !== undefined)
                {
                    var item = itemWaitingOn

                    if (mouseX > item.x && mouseX < item.x + constants.cellWidth) // We entered corner of other item, starting timer
                        isAimingOnStacking = true
                    else
                        isAimingOnStacking = false

                    xWaiting = mouseX
                    yWaiting = mouseY
                }

            }

            onTriggered: {
                if (itemWaitingOn !== undefined && model.get(gridMouseArea.dndDest) !== undefined)
                {
                    var item = itemWaitingOn
                    var isHitInnerIcon = gridMouseArea.mouseX > item.x && gridMouseArea.mouseX < item.x + constants.cellWidth
                    var isDragginStack = model.get(gridMouseArea.dndDest).stack !== undefined

                    //var pointsDistance = Math.sqrt(Math.pow(gridMouseArea.mouseX - xWaiting, 2) + Math.pow(gridMouseArea.mouseY - yWaiting, 2))
                    //console.log("distance: " + pointsDistance)

                    if (stackable && gridMouseArea.draggedItemStackedAt !== undefined && (gridMouseArea.draggedItemStackedAt !== indexWaitingOn || !isHitInnerIcon) && !isDragginStack)
                    { // Unstacking if item we are above is not the one we stacked to
                        //console.log("UNSTACKING " + gridMouseArea.dndDest + " FROM " + indexWaitingOn)
                        grid.unstackItemInItem(gridMouseArea.draggedItemStackedAt, gridMouseArea.dndDest)
                        gridMouseArea.draggedItemStackedAt = undefined
                    }
                    else if (stackable && isHitInnerIcon && indexWaitingOn != gridMouseArea.dndDest && !isDragginStack) //&& pointsDistance <= 3)
                    { // Hit central part of item. Using for stacking
                        if (isAimingOnStacking)
                        {
                            //console.log("----------------- STACKING " + gridMouseArea.dndDest + " to " + indexWaitingOn)
                            var res = grid.stackItemInItem(indexWaitingOn, gridMouseArea.dndDest)
                            if (res)
                            {
                                gridMouseArea.draggedItemStackedAt = indexWaitingOn

                                //console.log("set " + indexWaitingOn + " with " + model.get(indexWaitingOn).stack.length + " at real pos " + model.get(indexWaitingOn).id)
                                if (gridMouseArea.dndDest > indexWaitingOn)
                                {
                                    gridMouseArea.dndDestId = count - 1
                                    model.move(gridMouseArea.dndDest, gridMouseArea.dndDestId, 1)
                                    currentIndex = gridMouseArea.draggedItemStackedAt
                                    gridMouseArea.dndDest = count - 1
                                }
                            }
                        }
                    }
                    else if (!isAimingOnStacking || !stackable || isDragginStack) // Hit outer part of item. Using for repositioning
                    {
                        //console.log("MOVING")

                        gridMouseArea.dndDestId = indexWaitingOn // This could had broken dnd-dndId connection but fixed bug
                        model.move(gridMouseArea.dndDest, indexWaitingOn, 1)
                        gridMouseArea.dndDest = indexWaitingOn
                        currentIndex = gridMouseArea.dndDest
                    }

                    itemWaitingOn = undefined
                }
            }
        }


        onMousePositionChanged: {
            if (!grid.moving && dndSrcId == -1)
            {
                // Optimize later to lesser use of getItemUnderCursor(true)
                var newCurrentIndex = getItemUnderCursor(!grid.myActiveFocus).index
                if (newCurrentIndex != -1 && (newCurrentIndex != currentIndex || !grid.activeFocus))
                {
                    if (!grid.myActiveFocus)
                    {
                        grid.highlightMoveDuration = 1
                        grid.currentIndex = newCurrentIndex
                        grid.forceMyFocus()
                        grid.highlightMoveDuration = highlightMoveDurationConst
                    }
                    else
                        grid.currentIndex = newCurrentIndex
                }
            }
            else if (dndSrcId != -1)
            {
                // activate system DnD if icons isn't group
                if (enabledSystemDnD && (mouseX < 0 || mouseX > width || mouseY < 0 || mouseY > height) &&
                        model.get(dndDest).stack === undefined) {
                    var url = dataSource.itemUrlDnd(dndSrcId, model.get(dndSrc).group)
                    if (url) {
                        // Return dragged icon to home position & disable internal DnD
                        model.move(dndDest, dndSrc, 1)
                        var imagePath = model.get(dndSrc).imagePath
                        dndStateChanged(false)
                        dndSrcId = -1
                        dndSrc = -1
                        dndDest = -1
                        dndDestId = -1
                        // start system DnD
                        mainWindow.close()
                        mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                        return
                    }
                }

                if (mouseY < -dragOutTopMargin || mouseY > grid.height + dragOutBottomMargin)
                {
                    if (groupRoot !== undefined && groupRoot.isPopupGroup !== undefined && groupRoot.isPopupGroup)
                    {
                        console.log("OUT")
                        draggedOut(model.get(gridMouseArea.dndDest))
                        // onReleased():
                        dndSrcId = -1
                        dndStateChanged(false)
                    }
                }

                var index = grid.getCellIndex(mouseX, mouseY)
                var wasCurrent = grid.currentIndex
                grid.currentIndex = index
                var item = grid.currentItem
                grid.currentIndex = wasCurrent

                mouseHoverTimer.itemWaitingOn = item
                mouseHoverTimer.indexWaitingOn = index
                mouseHoverTimer.calculateExpectations(mouseX, mouseY)
                mouseHoverTimer.start()
            }
        }

        onPressed: {
            pressedOnIndex = getItemUnderCursor(true).index
        }

        onPressAndHold: {
            if (draggable)
            {
                var index = getItemUnderCursor(true).index
                if (pressedOnIndex == index)
                    grid.startDragging(index)
            }
            else if (enabledSystemDnD)
            {
                var icon_index = getItemUnderCursor(true).index
                if (icon_index != -1 && pressedOnIndex == icon_index && model.get(icon_index).stack === undefined) {
                    var url = dataSource.itemUrlDnd(model.get(icon_index).id, model.get(icon_index).group)
                    if (url) {
                        var imagePath = model.get(icon_index).imagePath
                        // start system DnD
                        mainWindow.close()
                        mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                    }
                }
            }

        }
        onReleased: {
            var dndSrcIdSaved = dndSrcId

            if (gridMouseArea.draggedItemStackedAt !== undefined && model.get(gridMouseArea.dndDest).stack === undefined)
            {
                //console.log("STACK UPPED")

                if (dndDest < gridMouseArea.draggedItemStackedAt)
                {
                    gridMouseArea.dndDestId = count - 1
                    model.move(gridMouseArea.dndDest, gridMouseArea.dndDestId, 1)
                    //currentIndex = gridMouseArea.draggedItemStackedAt
                    gridMouseArea.dndDest = count - 1
                }

                model.remove(dndDest)

                //dndSrcId = -1; - this is intentionally commented out 'cause it's done in delegate's remove animation
                dndStateChanged(false)
            }
            else
            {
                dndSrcId = -1

                if (dndSrcIdSaved != -1)
                {
                    dndStateChanged(false)

                    if (typeof dataSource.itemDragged !== "undefined" && dndDestId != -1)
                    {
                        //console.log("dndDestId: " + dndDestId)
                        // FIXME: FIX THIS!!!!!!!!!!!!!!!!!
                        /*dataSource.itemDragged(dndSrcIdSaved, dndDestId)

                        model.set(dndDest, {"id": dndDestId})

                        if (dndDest < dndSrc)
                        {
                            for (var i = dndDest + 1; i <= dndSrc; i++)
                                model.set(i, {"id": model.get(i).id + 1})
                        }
                        else
                        {
                            for (var i = dndSrc; i < dndDest; i++)
                                model.set(i, {"id": model.get(i).id - 1})
                        }*/
                    }
                }
            }

            gridMouseArea.draggedItemStackedAt = undefined


            // Duplicates detection. Remove later when sure no duplication occurs
            var Set = function() {}
            Set.prototype.add = function(o) { this[o] = o; }
            Set.prototype.remove = function(o) { delete this[o]; }
            var ids = new Set
            for (var j = 0; j < model.count; j++)
            {
                if (model.get(j).id in ids)
                    console.log("!!!!!!!!!!!!!!!!!!!!! DUPLICATE ID DETECTED!!!!: " + model.get(j).id + " - " + model.get(j).caption + " | " + (model.get(j).stack === undefined) + "; count: " + j + " | TAKEN BY: " + ids[model.get(j).id].caption)
                else
                {
                    ids.add(model.get(j).id)
                    ids[model.get(j).id].caption = model.get(j).caption
                }
            }

            //    console.log(model.get(j).caption + " | " + model.get(i).id + " | " + i)

        }

        onClicked: {
            if (!grid.moving)
            {
                var indexClicked = getItemUnderCursor(true).index
                model.itemClicked(indexClicked)
            }
        }
    }
}

