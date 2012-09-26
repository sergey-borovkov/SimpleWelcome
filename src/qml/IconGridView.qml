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

    property alias dndSrcId: gridMouseArea.dndSrcId

    signal dndStateChanged(bool isDrag)
    signal selectionChangedByKeyboard(variant newCurrentItem)

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

    function newItemData(iconPath, name, itemId) {
        appsModel.append( { imagePath: iconPath, caption: name, id: itemId, pinned: undefined, stack: undefined })
    }

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

    function stackItemInItem(indexStackingTo, indexDragging) {
        //console.log("----------------- STACKING " + gridMouseArea.dndDest + " to " + indexWaitingOn)

        var itemDragging = model.get(indexDragging)
        //console.log(gridMouseArea.dndDest + " with " + itemDragging)
        var itemStackingTo = model.get(indexStackingTo)

        var stackArray = itemStackingTo.stack
        if (stackArray === undefined) {
            //console.log("FIRST TIME STACKING")
            stackArray = []

            // Copying object by value
            var newObj = new Object
            for (var s in itemStackingTo) {
                //console.log("copying " + itemStackingTo[s])
                newObj[s] = itemStackingTo[s]
            }

            stackArray.push(newObj)
        }
        else {
            console.log("STACKING AGAIN")
            for (var i in stackArray) // Checking if item is already present in stack
                if (stackArray[i].id === itemDragging.id)
                {
                    //console.log("Duplicate ignored")
                    return false
                }
        }
        stackArray.push(itemDragging)

        model.setProperty(indexStackingTo, "imagePath", "image://generalicon/stacked/" + itemStackingTo.imagePath.slice(28) + "|" + itemDragging.imagePath.slice(28))
        model.setProperty(indexStackingTo, "stack", stackArray)
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
        for (var i in stackArray) // Checking if item is already present in stack
            if (stackArray[i].id === itemDragging.id)
                stackArray.splice(i)
            else
                stackIcon += stackArray[i].imagePath.slice(28) + "|"

        if (stackArray.length === 1) {
            //console.log("LAST ELEMENT LEFT")
            var innerItem = itemUnstackingFrom.stack[0]
            model.setProperty(indexUnstackingFrom, "stack", undefined)
            model.set(indexUnstackingFrom, innerItem)
        }
        else {
            //console.log("WAS: " + itemUnstackingFrom.imagePath)
            //console.log("TRUNCATING: " + itemDragging.imagePath)
            //console.log("NOEW: " + itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "imagePath", itemUnstackingFrom.imagePath.substring(0, itemUnstackingFrom.imagePath.length - (itemDragging.imagePath.length - 28 + 1)))
            model.setProperty(indexUnstackingFrom, "stack", stackArray)
        }
        return true
    }

    Component.onCompleted: {
        //console.log("COMPLETED " + dataSource + " VIEW")
        if (dataSource)
        {
            dataSource.newItemData.connect(newItemData)
            dataSource.resetContent.connect(resetContent)
            dataSource.getContent()
        }
        appsModel.itemClicked.connect(onItemClicked)
    }


    onActiveFocusChanged: {
        if (highlightItem) // we are not empty and we have selection rectangle
        {
            if (!activeFocus)
                highlightItem.opacity = 0
            else
                highlightItem.opacity = 1
        }
    }

    //Keys.forwardTo: topBar
    Keys.onPressed: {
        var newCurrentItem
        switch (event.key)
        {
        case Qt.Key_Left:
            if (currentIndex == 0 && prevGrid)
            {
                prevGrid.highlightMoveDuration = 1
                prevGrid.currentIndex = prevGrid.count - 1
                prevGrid.forceActiveFocus()
                prevGrid.highlightMoveDuration = highlightMoveDurationConst
                newCurrentItem = prevGrid.currentItem
            }

            if (!interactive)
            {
                moveCurrentIndexLeft()
                event.accepted = true
            }
            break
        case Qt.Key_Right:
            if (currentIndex == count - 1 && nextGrid)
            {
                nextGrid.highlightMoveDuration = 1
                nextGrid.currentIndex = 0
                nextGrid.forceActiveFocus()
                nextGrid.highlightMoveDuration = highlightMoveDurationConst
                newCurrentItem = nextGrid.currentItem
            }

            if (!interactive)
            {
                moveCurrentIndexRight()
                event.accepted = true
            }
            break
        case Qt.Key_Up:
            if (currentIndex < columns && prevGrid)
            {
                var roundCount = Math.floor((prevGrid.count) / columns) * columns
                var newCur = (currentIndex % columns) + roundCount - columns * Math.min(1, Math.floor((currentIndex % columns) / (prevGrid.count - roundCount)))

                prevGrid.highlightMoveDuration = 1
                prevGrid.currentIndex = newCur
                prevGrid.forceActiveFocus()
                prevGrid.highlightMoveDuration = highlightMoveDurationConst
                newCurrentItem = prevGrid.currentItem
            }

            if (!interactive)
            {
                moveCurrentIndexUp()
                event.accepted = true
            }
            break
        case Qt.Key_Down:
            if (currentIndex >= count - columns && nextGrid)
            {
                nextGrid.highlightMoveDuration = 1
                nextGrid.currentIndex = currentIndex % columns
                nextGrid.forceActiveFocus()
                nextGrid.highlightMoveDuration = highlightMoveDurationConst
                newCurrentItem = nextGrid.currentItem
            }

            if (!interactive)
            {
                moveCurrentIndexDown()
                event.accepted = true
            }
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

        if (event.key == Qt.Key_Left || event.key == Qt.Key_Right ||
            event.key == Qt.Key_Up || event.key == Qt.Key_Down)
            selectionChangedByKeyboard(newCurrentItem == null ? currentItem : newCurrentItem)
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
            interval: 300
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
                if (itemWaitingOn !== undefined)
                {
                    var item = itemWaitingOn
                    var isHitInnerIcon = gridMouseArea.mouseX > item.x && gridMouseArea.mouseX < item.x + constants.cellWidth
                    var isDragginStack = model.get(gridMouseArea.dndDest).stack !== undefined

                    //var pointsDistance = Math.sqrt(Math.pow(gridMouseArea.mouseX - xWaiting, 2) + Math.pow(gridMouseArea.mouseY - yWaiting, 2))
                    //console.log("distance: " + pointsDistance)

                    if (gridMouseArea.draggedItemStackedAt !== undefined && (gridMouseArea.draggedItemStackedAt !== indexWaitingOn || !isHitInnerIcon) && !isDragginStack)
                    { // Unstacking if item we are above is not the one we stacked to
                        console.log("UNSTACKING " + gridMouseArea.dndDest + " FROM " + indexWaitingOn)
                        grid.unstackItemInItem(gridMouseArea.draggedItemStackedAt, gridMouseArea.dndDest)
                        gridMouseArea.draggedItemStackedAt = undefined
                    }
                    else if (isHitInnerIcon && indexWaitingOn != gridMouseArea.dndDest && !isDragginStack) //&& pointsDistance <= 3)
                    { // Hit central part of item. Using for stacking
                        if (isAimingOnStacking)
                        {
                            console.log("----------------- STACKING " + gridMouseArea.dndDest + " to " + indexWaitingOn)
                            var res = grid.stackItemInItem(indexWaitingOn, gridMouseArea.dndDest)
                            if (res)
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
                    else if (!isAimingOnStacking) // Hit outer part of item. Using for repositioning
                    {
                        console.log("MOVING")

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
                var newCurrentIndex = getItemUnderCursor(!grid.activeFocus).index
                if (newCurrentIndex != -1 && (newCurrentIndex != currentIndex || !grid.activeFocus))
                {
                    if (!grid.activeFocus)
                    {
                        grid.highlightMoveDuration = 1
                        grid.currentIndex = newCurrentIndex
                        grid.forceActiveFocus()
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
                        mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                        return
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

                //console.log("x: " + curColumn + "| Mouse: " + mouseX)
                //console.log("c: " + curIndex + "| Mouse: " + mouseY)

                /*var itemUnderCursor = getItemUnderCursor(true)
                var index = itemUnderCursor.index
                var item = itemUnderCursor.item
                //console.log("ITEM: " + itemUnderCursor)

                //if (item)

                if (index != -1 && index != dndDest)
                {

                    if (mouseX > item.x + item.width / 3 && mouseX < item.x + item.width - item.width / 3) // We entered corner of other item, starting timer
                    {
                        mouseHoverTimer.itemWaitingOn = item
                        mouseHoverTimer.indexWaitingOn = index
                        mouseHoverTimer.start()
                    }
                    else
                    {
                    }
                }*/
            }
        }

        onPressed: {
            pressedOnIndex = getItemUnderCursor(true).index
        }

        onPressAndHold: {
            if (draggable)
            {
                var index = getItemUnderCursor(true).index
                if (index != -1 && pressedOnIndex == index)
                {
                    dndDest = index
                    dndSrc = index
                    dndSrcId = model.get(dndSrc).id
                    dndDestId = dndSrcId
                    //console.log("dndSrc, dndSrcId, dndDest, dndDestId: " + dndSrc + " " + dndSrcId + " " + dndDest + " " + dndDestId)
                    dndStateChanged(true)

//                    console.log("NOW----------------")
//                    for (var i = 0; i < model.count; i++)
//                        console.log(model.get(i).caption + " | " + model.get(i).id + " | " + i)
//                    console.log("END----------------")
                }
            }
            else if (enabledSystemDnD)
            {
                var icon_index = getItemUnderCursor(true).index
                if (icon_index != -1 && pressedOnIndex == icon_index && model.get(icon_index).stack === undefined) {
                    var url = dataSource.itemUrlDnd(model.get(icon_index).id, model.get(icon_index).group)
                    if (url) {
                        var imagePath = model.get(icon_index).imagePath
                        // start system DnD
                        mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                    }
                }
            }

        }
        onReleased: {
            var dndSrcIdSaved = dndSrcId

            if (gridMouseArea.draggedItemStackedAt !== undefined && model.get(gridMouseArea.dndDest).stack === undefined)
            {
                //model.remove(dndDest)
                console.log("STACK UPPED")
                model.setProperty(dndDest, "hidden", true)
                //model.setProperty(gridMouseArea.draggedItemStackedAt, "id", 100500)

                if (dndDest < gridMouseArea.draggedItemStackedAt)
                {
                    gridMouseArea.dndDestId = count - 1
                    model.move(gridMouseArea.dndDest, gridMouseArea.dndDestId, 1)
                    //currentIndex = gridMouseArea.draggedItemStackedAt
                    gridMouseArea.dndDest = count - 1
                }

                dndSrcId = -1
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
                        dataSource.itemDragged(dndSrcIdSaved, dndDestId)

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
                        }
                    }
                }
            }
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

