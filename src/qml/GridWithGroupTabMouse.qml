import QtQuick 1.1

MouseArea {
    id: gridMouseArea
    anchors.fill: gridsListView
    hoverEnabled: true

    property int dndSrcId: -1
    property int dndSrc: -1
    property int dndAbsoluteSrc: -1
    property int dndDest: -1
    property int pressedOnIndex
    property variant draggedItemStackedAt
    property variant cachedGrid
    property variant grid
    property bool skipMoveAnimation: false

    property real gridMouseY: grid ? mapToItem(grid, 0, mouseY).y : 0
    property real gridMouseX

    property int gridMouseXWas // used when dragging icon between tabs

    function getGridMouseX() {
        return grid ? mapToItem(grid, mouseX, 0).x : 0
    }

    Binding
    {
        id: gridMouseXBinding
        property bool enabled: true
        target: gridMouseArea
        property: "gridMouseX"
        value: getGridMouseX()
        when: enabled
    }

    function hitTest(bound, coords) {
        return coords.x >= 0 && coords.y >= 0 && coords.x <= bound.width && coords.y <= bound.height
    }

    function updateCurrentGrid() {
        if (popupFrame.state === "OPEN") {
            grid = popupFrame.gridGroup.gridView
            return
        }

        if (cachedGrid !== undefined && cachedGrid) {
            var cachedGridCoords = mapToItem(cachedGrid, mouseX, mouseY)
            if (!cachedGrid.mouseDragChangesGrids && pressed || hitTest(cachedGrid, cachedGridCoords)) {
                //console.log("returning cached " + cachedGrid)
                grid = cachedGrid
                return
            }
        }
        //console.log("request grid at " + mouseX + ":" + mouseY)
        if (gridsListView.currentItem && !gridsListView.moving) {
            var childs = gridsListView.currentItem.children

            // Iterating by it's GridWithGroups
            for (var child = 0; child < childs.length; child++) {
                if ('gridView' in childs[child])
                {
                    var gridView = childs[child].gridView
                    var coords = mapToItem(gridView, mouseX, mouseY)
                    if (hitTest(gridView, coords)) {
                        cachedGrid = gridView
                        grid = cachedGrid
                        return
                    }
                }
            }
        }

        grid = cachedGrid
    }

    function getItemUnderCursor(isForceRecheck)
    {
        var wasCurrentIndex = grid.currentIndex
        var mouseXReal = gridMouseX + grid.contentX, mouseYReal = gridMouseY + grid.contentY
        var wasContentX = grid.contentX, wasContentY = grid.contentY
        var indexUnderMouse = grid.indexAt(mouseXReal, mouseYReal)
        var result = new Object
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

    function startDragging(index) {
        if (index !== -1)
        {
            dndDest = index
            dndSrc = index
            dndAbsoluteSrc = index + grid.indexStartAt
            dndSrcId = grid.model.get(index).id
            gridsListView.dndStateChanged(true)
            //console.log("grid: " + grid)

//                    console.log("NOW----------------")
//                    for (var i = 0; i < model.count; i++)
//                        console.log(model.get(i).caption + " | " + model.get(i).id + " | " + i)
//                    console.log("END----------------")
        }
    }

    function dragIconToPrevNextTab(isForward) {
        if (isForward && tabListView.currentIndex < tabListView.count - 1 ||
                !isForward && tabListView.currentIndex > 0) {
            var itemMoved = root.cloneObject(grid.model.get(dndDest))
            gridMouseArea.skipMoveAnimation = true

            if (isForward)
                tabListView.incrementCurrentIndex()
            else
                tabListView.decrementCurrentIndex()

            var nextContainer = tabListView.currentItem
            if (isForward)
                tabListView.decrementCurrentIndex()
            else
                tabListView.incrementCurrentIndex()

            var nextPageModel, nextPageGrid
            var childs = nextContainer.children

            // Iterating by it's GridWithGroups
            for (var child = 0; child < childs.length; child++)
                if ('gridView' in childs[child] && childs[child].groupName === "Apps")
                {
                    nextPageGrid = childs[child].gridView
                    nextPageModel = childs[child].gridView.model
                    break
                }

            var itemFromNextPage
            if (isForward)
                itemFromNextPage = nextPageModel.get(0)
            else
                itemFromNextPage = nextPageModel.get(nextPageModel.count - 1)

            var dndDestWas = dndDest
            var gridWas = grid
            dndSrc = nextPageModel.count - 1
            dndDest = dndSrc
            dndSrcId = itemMoved.id

            // FIXME: Add logic to append new page when current one is overflown
            if (isForward)
                tabListView.incrementCurrentIndex()
            else
                tabListView.decrementCurrentIndex()

            cachedGrid = nextPageGrid

            // A hack to use correct mouse coordinates when tabListView is scrolled
            gridMouseXBinding.enabled = false
            //console.log("gridMouseXWas", gridMouseXWas, "now:", gridMouseX)
            gridMouseX = gridMouseXWas

            mousePosChanged()
            gridMouseXBinding.enabled = true


            if (gridWas.stackable && draggedItemStackedAt !== undefined) {
                // Unstacking first
                gridWas.unstackItemInItem(draggedItemStackedAt, dndDestWas)
                draggedItemStackedAt = undefined
            }

            gridWas.model.remove(dndDestWas)

            if (isForward) {
                if (itemFromNextPage)
                    gridWas.model.append(itemFromNextPage)
                nextPageModel.remove(0)
                nextPageModel.append(itemMoved)
            }
            else {
                if (itemFromNextPage)
                    gridWas.model.insert(0, itemFromNextPage)
                nextPageModel.remove(nextPageModel.count - 1)
                nextPageModel.append(itemMoved)
            }
            gridMouseArea.skipMoveAnimation = false
        }
    }


    Timer {
        id: tabsSwitchingTimer
        repeat: true

        property bool isForward
        property int firstInterval: 300
        property int nextInterval: 800

        property int cornerZone: 15

        onTriggered: {
            interval = nextInterval

            if (grid.mouseDragChangesGrids) {
                if (isForward && gridMouseArea.mouseX <= tabListView.width - cornerZone ||
                        !isForward && gridMouseArea.mouseX >= cornerZone) {
                    stop()
                    return
                }

                dragIconToPrevNextTab(isForward)
            }
        }
    }

    Timer {
        id: mouseHoverTimer
        interval: grid === undefined || grid.stackable ? 300 : 0
        property variant itemWaitingOn: undefined
        property variant indexWaitingOn: undefined
        property bool isAimingOnStacking
        property real xWaiting
        property real yWaiting

        function calculateExpectations(gridMouseX, gridMouseY) {
            if (itemWaitingOn !== undefined)
            {
                var item = itemWaitingOn

                if (gridMouseX > item.x && gridMouseX < item.x + constants.cellWidth) // We entered corner of other item, starting timer
                    isAimingOnStacking = true
                else
                    isAimingOnStacking = false

                xWaiting = gridMouseX
                yWaiting = gridMouseY
            }

        }

        onTriggered: {
            if (grid && itemWaitingOn && grid.model.get(gridMouseArea.dndDest) !== undefined && gridMouseArea.pressed)
            {
                var item = itemWaitingOn
                var isHitInnerIcon = gridMouseArea.gridMouseX > item.x && gridMouseArea.gridMouseX < item.x + constants.cellWidth
                var isDragginStack = grid.model.get(gridMouseArea.dndDest).stack !== undefined

                //var pointsDistance = Math.sqrt(Math.pow(gridMouseArea.gridMouseX - xWaiting, 2) + Math.pow(gridMouseArea.gridMouseY - yWaiting, 2))
                //console.log("distance: " + pointsDistance)

                if (grid.stackable && gridMouseArea.draggedItemStackedAt !== undefined && (gridMouseArea.draggedItemStackedAt !== indexWaitingOn || !isHitInnerIcon) && !isDragginStack)
                { // Unstacking if item we are above is not the one we stacked to
                    //console.log("UNSTACKING " + gridMouseArea.dndDest + " FROM " + indexWaitingOn)
                    grid.unstackItemInItem(gridMouseArea.draggedItemStackedAt, gridMouseArea.dndDest)
                    gridMouseArea.draggedItemStackedAt = undefined
                }
                else if (grid.stackable && isHitInnerIcon && indexWaitingOn != gridMouseArea.dndDest && !isDragginStack) //&& pointsDistance <= 3)
                { // Hit central part of item. Using for stacking
                    if (isAimingOnStacking)
                    {
                        //console.log("----------------- STACKING " + gridMouseArea.dndDest + " to " + indexWaitingOn)
                        var res = grid.stackItemInItem(indexWaitingOn, gridMouseArea.dndDest)
                        if (res)
                        {
                            gridMouseArea.draggedItemStackedAt = indexWaitingOn

                            //console.log("set " + indexWaitingOn + " with " + grid.model.get(indexWaitingOn).stack.length + " at real pos " + grid.model.get(indexWaitingOn).id)
                            if (gridMouseArea.dndDest > indexWaitingOn)
                            {
                                grid.model.move(gridMouseArea.dndDest, grid.count - 1, 1)
                                grid.currentIndex = gridMouseArea.draggedItemStackedAt
                                gridMouseArea.dndDest = grid.count - 1
                            }
                        }
                    }
                }
                else // if (!isAimingOnStacking && !grid.stackable || isDragginStack) // Hit outer part of item. Using for repositioning
                {
                    //console.log("MOVING")

                    grid.model.move(gridMouseArea.dndDest, indexWaitingOn, 1)
                    gridMouseArea.dndDest = indexWaitingOn
                    grid.currentIndex = gridMouseArea.dndDest
                }

                itemWaitingOn = undefined
            }
        }
    }

    function mousePosChanged() {
        updateCurrentGrid()
        if (grid === undefined || !grid)
            return

        if (gridMouseX >= 0 && gridMouseX < width) {
            gridMouseXWas = gridMouseX
//            console.log("update gridMouseXWas to", gridMouseX)
        }

        //console.log("x: " + gridMouseX + "; y: " + gridMouseY + " " + grid)

        if (!grid.moving && dndSrcId === -1)
        {
            // Optimize later to lesser use of getItemUnderCursor(true)
            var newCurrentIndex = getItemUnderCursor(!grid.myActiveFocus).index
            if (newCurrentIndex !== -1 && (newCurrentIndex !== grid.currentIndex || !grid.myActiveFocus))
            {
                if (!grid.myActiveFocus)
                    grid.selectOtherGrid(grid, newCurrentIndex)
                else
                    grid.currentIndex = newCurrentIndex
            }
        }
        else if (dndSrcId != -1)
        {
            // activate system DnD if icons isn't group
            if (grid.enabledSystemDnD && gridMouseY > height &&
                    grid.model.get(dndDest).stack === undefined) {

                var url = grid.dataSource.itemUrlDnd(dndSrcId, grid.model.get(dndSrc).group)
                if (url) {
                    // Return dragged icon to home position & disable internal DnD
                    if (grid.stackable && draggedItemStackedAt !== undefined) {
                        // Unstacking before starting of system D&D
                        grid.unstackItemInItem(draggedItemStackedAt, dndDest)
                        draggedItemStackedAt = undefined
                    }
                    grid.model.move(dndDest, dndSrc, 1)

                    var imagePath = grid.model.get(dndSrc).imagePath
                    gridsListView.dndStateChanged(false)
                    dndSrcId = -1
                    dndSrc = -1
                    dndDest = -1
                    // start system DnD
                    mainWindow.close()
                    mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                    return
                }
            }

            // In case group is popup - check necessity to drag out of popup
            if (grid.isPopupGroup && (gridMouseY < -grid.dragOutTopMargin || gridMouseY > grid.height + grid.dragOutBottomMargin))
                tabWrapper.draggedOut(grid.model.get(dndDest))
            // If hit right corner of screen - scrolling to next screen
            else if (grid.mouseDragChangesGrids && mouseX > tabListView.width - tabsSwitchingTimer.cornerZone) {
                if (!tabsSwitchingTimer.running || !tabsSwitchingTimer.isForward)
                {
                    tabsSwitchingTimer.interval = tabsSwitchingTimer.firstInterval
                    tabsSwitchingTimer.isForward = true
                    gridMouseArea.gridMouseXWas = gridMouseX
                    tabsSwitchingTimer.start()
                }
            }
            // If hit left corner of screen - scrolling to previous screen
            else if (grid.mouseDragChangesGrids && mouseX < tabsSwitchingTimer.cornerZone) {
                if (!tabsSwitchingTimer.running || tabsSwitchingTimer.isForward)
                {
                    tabsSwitchingTimer.interval = tabsSwitchingTimer.firstInterval
                    tabsSwitchingTimer.isForward = false
                    gridMouseArea.gridMouseXWas = gridMouseX
                    tabsSwitchingTimer.start()
                }
            }

            var index = grid.getCellIndex(gridMouseX, gridMouseY)
            var wasCurrent = grid.currentIndex
            grid.currentIndex = index
            var item = grid.currentItem
            grid.currentIndex = wasCurrent

            mouseHoverTimer.itemWaitingOn = item
            mouseHoverTimer.indexWaitingOn = index
            mouseHoverTimer.calculateExpectations(gridMouseX, gridMouseY)
            mouseHoverTimer.start()
        }
    }


    onMousePositionChanged: mousePosChanged()


    onPressed: {
        pressedOnIndex = getItemUnderCursor(true).index
    }

    onPressAndHold: {
        if (grid.draggable)
        {
            var index = getItemUnderCursor(true).index
            if (pressedOnIndex === index)
                startDragging(index)
        }
        else if (grid.enabledSystemDnD)
        {
            var icon_index = getItemUnderCursor(true).index
            if (icon_index !== -1 && pressedOnIndex === icon_index && grid.model.get(icon_index).stack === undefined) {
                var url = grid.dataSource.itemUrlDnd(grid.model.get(icon_index).id, grid.model.get(icon_index).group)
                if (url) {
                    var imagePath = grid.model.get(icon_index).imagePath
                    // start system DnD
                    mainWindow.close()
                    mainWindow.activateDragAndDrop(url, imagePath, constants.iconSize)
                }
            }
        }

    }
    onReleased: {
        skipMoveAnimation = false
        tabsSwitchingTimer.stop()

        var dndSrcIdSaved = dndSrcId
        //console.log("RELEASED")

        // Adding icon to stack
        if (draggedItemStackedAt !== undefined && grid.model.get(dndDest).stack === undefined) {
            //console.log("STACK UPPED")

            var container = grid.model.get(draggedItemStackedAt)
            if (container.stack.length === 2) { // First time stacking
                console.log("draggedItemStackedAt: " + draggedItemStackedAt)
                console.log("Adding move of " + container.caption + " from "  + (grid.indexStartAt + draggedItemStackedAt) + " to " + (grid.indexStartAt + draggedItemStackedAt))
                gridsListView.itemMoved(container.caption, grid.indexStartAt + draggedItemStackedAt, grid.indexStartAt + draggedItemStackedAt)
            }

            if (dndDest < draggedItemStackedAt) {
                grid.model.move(dndDest, grid.count - 1, 1)
                //grid.currentIndex = draggedItemStackedAt
                dndDest = grid.count - 1
            }

            gridsListView.itemMoved(grid.model.get(dndDest).caption, dndAbsoluteSrc, -1)


            grid.model.remove(dndDest)

            dndSrcId = -1
            gridsListView.dndStateChanged(false)
        }
        else {
            dndSrcId = -1

            if (dndSrcIdSaved != -1) {
                gridsListView.dndStateChanged(false)

                if (dndAbsoluteSrc !== dndDest + grid.indexStartAt) {
                    //console.log("SAVING ICON POSITION: #" + dndSrcIdSaved + " - " + grid.model.get(dndDest).caption + " in " + dndDest + "; dndSrc:" + dndSrc + "; dndDest: " + dndDest + " | " + grid.indexStartAt)
                    gridsListView.itemMoved(grid.model.get(dndDest).caption, dndAbsoluteSrc, grid.indexStartAt + dndDest)
                }

                // Sync icons order in C++ model to QML model. Used in Recent Apps
                if (typeof grid.dataSource.itemDragged !== "undefined" && dndDest != -1) {
                    console.log("REARRANGING C++")
                    //console.log("dndSrc, dndSrcId, dndDest: " + dndSrc + " " + dndSrcId + " " + dndDest)

                    grid.dataSource.itemDragged(dndSrcIdSaved, dndDest)
                    grid.model.setProperty(dndDest, "id", dndDest)

                    var i
                    if (dndDest < dndSrc) {
                        for (i = dndDest + 1; i <= dndSrc; i++)
                            grid.model.setProperty(i, "id", grid.model.get(i).id + 1)
                    }
                    else {
                        for (i = dndSrc; i < dndDest; i++)
                            grid.model.setProperty(i, "id", grid.model.get(i).id - 1)
                    }
                }
            }
        }

        draggedItemStackedAt = undefined

        if (grid.maxCount !== -1 && dndSrcIdSaved !== -1 && grid.count !== grid.maxCount) {
            gridsListView.gridIconPushPop("Apps")
        }

        // Duplicates detection. Remove later when sure no duplication occurs
        /*var Set = function() {}
        Set.prototype.add = function(o) { this[o] = o; }
        Set.prototype.remove = function(o) { delete this[o]; }
        var ids = new Set
        for (var j = 0; j < grid.model.grid.count; j++)
        {
            if (grid.model.get(j).id in ids)
                console.log("!!!!!!!!!!!!!!!!!!!!! DUPLICATE ID DETECTED!!!!: " + grid.model.get(j).id + " - " + grid.model.get(j).caption + " | " + (grid.model.get(j).stack === undefined) + "; grid.count: " + j + " | TAKEN BY: " + ids[grid.model.get(j).id].caption)
            else
            {
                ids.add(grid.model.get(j).id)
                ids[grid.model.get(j).id].caption = grid.model.get(j).caption
            }
        }*/

        //    console.log(grid.model.get(j).caption + " | " + grid.model.get(i).id + " | " + i)

    }

    onClicked: {
        if (grid.isPopupGroup && (gridMouseY < -grid.dragOutTopMargin || gridMouseY > grid.height + grid.dragOutBottomMargin))
            gridsListView.hideGroup()
        else if (!grid.moving)
        {
            var indexClicked = getItemUnderCursor(true).index
            grid.model.itemClicked(indexClicked)
        }
    }
}
