import QtQuick 1.1
import Private 0.1

Item {
    id: tabRoot
    anchors.fill: parent
    clip:true

    property variant groups
    property bool isForceOnOneScreen: false
    property alias tabListView: gridsListView
    property string isPopupOpened: popupFrame.state === "OPEN"
    property int stackCellOpenedId: -1

    property variant dataSources
    property variant iconPositions

    function updateGridsContent()
    {
        gridsListView.updateGridsContent()
    }

    function processKeyboard(key) {
        if (popupFrame.state === "OPEN") {
            popupFrame.gridGroup.gridView.processKeyboard(key)
        }
        else if (gridsListView.currentItem)
            gridsListView.currentItem.processKeyboard(key)
    }

    Item {
        id: tabWrapper
        width: parent.width
        height: parent.height + popupFrame.slideHeight
        anchors.top: parent.top
        anchors.topMargin: -popupFrame.slideHeight

        function draggedOut(item) {
            gridsListView.hideGroup(false)
            //console.log(item.caption + " GOT")
            if (gridsListView.currentItem)
                gridsListView.currentItem.activeGridGroup.state = "clipped"
            gridsListView.activeGridView.newItemData(item)
            gridsListView.activeGridView.unstackItemInItem(popupFrame.stackedIconIndex, gridsListView.activeGridView.count - 1)
            gridMouseArea.skipMoveAnimation = true
            gridMouseArea.updateCurrentGrid()
            gridMouseArea.startDragging(gridsListView.activeGridView.count - 1)
            popupFrame.stackedIconIndex = -1
        }

        ListView {
            id: gridsListView

            property variant activeGridView: currentItem ? currentItem.activeGridGroup.gridView : undefined

            anchors.top: parent.top
            anchors.topMargin: 16
            width: parent.width
            height: parent.height - 50

            snapMode: ListView.SnapOneItem
            orientation: ListView.Horizontal
            flickDeceleration: 5000
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 240
            cacheBuffer: 20000
            highlightRangeMode: ListView.StrictlyEnforceRange
            interactive: true

            Behavior on anchors.topMargin {
                NumberAnimation { duration: 600; /*easing.type: Easing.OutQuint*/ }
            }

            model: VisualDataModel {
                id: gridsVisualModel

                model: ListModel {
                    id: gridsListModel
                }

                delegate: GridWithGroupContainer {
                    width: gridsListView.width
                    height: gridsListView.height
                    defaultGroupData: defaultGroup
                }
            }

            function dndStateChanged(isDrag)
            {
                interactive = !isDrag
            }

            function gridIconPushPop(groupName) {
                //console.log("REINITING " + groupName)

                var currentIndexWas = gridsListView.currentIndex
                var prevPageModel = activeGridView.model
                var isPushingFurther = activeGridView.count > activeGridView.maxCount
                if (currentItem)
                    currentItem.activeGridGroup.state = isPushingFurther ? "unclipped" : "clipped"

                // Iterating by all GridWithGroupContainers
                for (var currentView = currentIndexWas + 1; currentView < gridsListView.count; currentView++) {
                    gridsListView.currentIndex = currentView

                    if (gridsListView.currentItem) {
                        var childs = gridsListView.currentItem.children

                        // Iterating by it's GridWithGroups
                        for (var child = 0; child < childs.length; child++) {
                            if ('gridView' in childs[child] && childs[child].groupName === groupName)
                            {
                                var nextPageModel = childs[child].gridView.model

                                if (isPushingFurther) {
                                    nextPageModel.insert(0, prevPageModel.get(prevPageModel.count - 1))
                                    prevPageModel.remove(prevPageModel.count - 1)
                                    // FIXME: Add logic to append new page when current one is overflown
                                }
                                else if (nextPageModel.count) {
                                    prevPageModel.append(nextPageModel.get(0))
                                    nextPageModel.remove(0)
                                }

                                prevPageModel = nextPageModel
                            }
                        }
                    }
                }
                gridsListView.currentIndex = currentIndexWas


//                var wasCurrent = gridsListView.currentIndex
//                updateGridsContent()
//                gridsListView.currentIndex = wasCurrent

            }

            function insertGrid(groupData, isOnNewTab)
            {
                var lastCountEnd = 0,
                    lastGridGroupName = undefined

                if (gridsListView.currentItem && gridsListView.currentItem.activeGridGroup !== undefined) {
                    lastCountEnd = gridsListView.currentItem.activeGridGroup.indexStartAt + gridsListView.currentItem.activeGridGroup.maxCount
                    lastGridGroupName = gridsListView.currentItem.activeGridGroup.groupName
                }

                if (isOnNewTab)
                {
                    if (lastGridGroupName !== undefined && lastGridGroupName === groupData.groupName)
                        groupData["indexStartAt"] = lastCountEnd

                    gridsListModel.append( { defaultGroup: groupData } )
                    gridsListView.currentIndex = count - 1
                }
                else
                {
                    gridsListView.currentItem.addGridGroup(groupData)
                }

                var newGridGroup = gridsListView.currentItem.activeGridGroup
                if (gridsListView.currentItem)
                {
                    newGridGroup.gridView.itemStackingChanged.connect(saveStacks)

                    if (newGridGroup.gridView.stackable)
                        newGridGroup.showPopupGroup.connect(showGroup)
                }
            }


            function createTabsFromGroups()
            {
                //if (groups !== undefined)
                //    //console.log("----------------------------------- createTabsFromGroups " + groups[0].groupName)
                //else
                //    //console.log("----------------------------------- createTabsFromGroups ")

                // Constants. Used hack to retrieve them from C++, no way to do it straightforward AFAIK
                var spacing = constants.gridWithGroupsSpacing, // spacing between GridWithGroups
                        columns = constants.gridColumns

                var availableHeight = 0
                //console.log("Starting to split " + availableHeight + "px to grids")
                for (var i = 0; i < groups.length; i++) // Iterating by grids
                {
                    var itemCount = groups[i].dataSource.getItemCount(groups[i].groupName)
                    if (!itemCount) // No need to add an empty group
                        continue

                    var cellRealHeight = groups[i].groupCellHeight !== undefined ? groups[i].groupCellHeight : constants.cellHeight
                    //console.log(i + " - NEW ITERATION!!!!!!!!! with " + groups[i].groupName)
                    var textHeight = (groups[i].groupNameVisible !== undefined && groups[i].groupNameVisible === false ? 0 : constants.groupTextHeight + constants.textToGridSpacing),
                            projectedGroupHeight = textHeight + Math.ceil(itemCount / columns) * cellRealHeight,
                            currentGroup = groups[i]
                    //console.log(i + " - Projected group height: " + projectedGroupHeight + "; Left here: " + availableHeight + ";  itemCount: " + itemCount)

                    if (projectedGroupHeight < availableHeight || (isForceOnOneScreen && count)) // Grid can be fully placed on the tab
                    {
                        //console.log(i + " - " + groups[i].dataSource + " is fitting the same screen");
                        currentGroup['maxCount'] = itemCount
                        insertGrid(currentGroup, false)
                        availableHeight -= projectedGroupHeight + spacing
                        //console.log(i + " - " + availableHeight + "px left");
                    }
                    else // Grid should be split or created new tab or both
                    {
                        //console.log(i + " - " + groups[i].groupName + " is going to be split or created new tab or both")
                        var rowsLeftToFit = Math.ceil(itemCount / columns)
                        var lastNotInsertedItem = 0

                        while (rowsLeftToFit > 0)
                        {
                            availableHeight -= textHeight
                            var rowsFit = Math.floor(availableHeight / cellRealHeight)
                            //console.log(i + " - Rows fit currently: " + rowsFit + "; availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight + "; " + (availableHeight / cellRealHeight))
                            if (rowsFit <= 0) // Current tab has no space left. Creating new tab
                            {
                                //console.log(i + " - " + "Tab [" + (gridsListView.count - 1) +  "] has no space left. Creating new tab")
                                availableHeight = gridsListView.height - textHeight
                                rowsFit = Math.min(rowsLeftToFit, Math.max(1, Math.floor(availableHeight / cellRealHeight)) ) // We definitely want to have at least one row on newly created tab
                                //console.log(i + " - availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight)
                                currentGroup['maxCount'] = Math.min(itemCount - lastNotInsertedItem, rowsFit * columns)
                                lastNotInsertedItem += rowsFit * columns
                                rowsLeftToFit -= rowsFit
                                //console.log(i + " - " + "Fitted "  + rowsFit + " rows")

                                //console.log("=== " + currentGroup.maxCount)
                                insertGrid(currentGroup, true)
                                availableHeight -= rowsFit * cellRealHeight + spacing
                            }
                            else // It's still possible to insert some rows to current tab. Inserting
                            {
                                //console.log(i + " - " + "It's still possible to insert some rows to current tab. Inserting " + rowsFit)

                                currentGroup['maxCount'] = Math.min(itemCount - lastNotInsertedItem, rowsFit * columns)
                                lastNotInsertedItem += rowsFit * columns
                                rowsLeftToFit -= rowsFit

                                //console.log(i + " - adding to current tab: " + currentGroup.group + ", " + currentGroup.dataSource + " [" + currentGroup.maxCount + "]")
                                insertGrid(currentGroup, false)
                                availableHeight -= rowsFit * cellRealHeight + spacing
                                //console.log(i + " - Taken " + rowsFit * cellRealHeight + "px; " + availableHeight)
                            }

                            //console.log(i + " - ROWS FITTING ITERATION END")
                        }
                    }
                }

                // Doing afterjob to load user icons positioning and stacks
                iconPositions = mainWindow.loadIconPositions() // FIXME: DO IT ONCE
            }

            function loadStacks(list, savedStacks) {
                //console.log("LOADING STACKS ")

                for (var stackName in savedStacks) {
                    //console.log("-----------")
                    //console.log("stackName: " + stackName)

                    var captionsList = savedStacks[stackName].split(",")
                    var captionStackingTo = captionsList[0]
                    //console.log("Object item:", captionStackingTo, "=", captionsList)

                    var indexStackingTo = -1
                    for (var i = 0; i < list.length; i++) {
                        if (list[i].caption === captionStackingTo) {
                            //console.log("LOADING " + list[i].caption + " checking with " + captionStackingTo)
                            indexStackingTo = i
                            break
                        }
                    }

                    if (indexStackingTo == -1)
                        continue

                    for (var captionToStackIndex = 0; captionToStackIndex < captionsList.length; captionToStackIndex++) {
                        var captionToStack = captionsList[captionToStackIndex]
                        if (captionToStack !== captionStackingTo) {
                            //console.log("NEED TO STACK: " + captionToStack)

                            var indexToStack = -1
                            for (var j = 0; j < list.length; j++)
                                if (list[j].caption === captionToStack) {
                                    indexToStack = j
                                    break
                                }

                            if (indexToStack != -1) {
                                gridsListView.stackItemInItem(list, indexStackingTo, indexToStack)
                                //console.log("SETTING " + list[indexStackingTo].caption + " caption to " + stackName)
                                list[indexStackingTo].caption = stackName

                                moveItemInArray(list, indexToStack, list.length - 1)

                                if (indexStackingTo > indexToStack)
                                    indexStackingTo--

                                list[list.length - 1].caption = ""
                            }
                        }
                    }

                }

                //console.log("LOADING STACKS FINISHED ---------------- ")
            }

            function saveStacks() {
                //console.log("function saveStacks()")
                var setting = []
                var currentIndexWas = gridsListView.currentIndex

                // Iterating by all GridWithGroupContainers
                for (var currentView = 0; currentView < gridsListView.count; currentView++) {
                    gridsListView.currentIndex = currentView

                    if (gridsListView.currentItem) {
                        var childs = gridsListView.currentItem.children

                        // Iterating by it's GridWithGroups
                        for (var child = 0; child < childs.length; child++) {
                            if ('gridView' in childs[child] && childs[child].groupName === "Apps")
                            {
                                var model = childs[child].gridView.model
                                //console.log("SAVING stacking")

                                for (var i = 0; i < model.count; i++) {
                                    var item = model.get(i)
                                    var stack = item.stack
                                    if (stack !== undefined) {
                                        setting.push(root.cloneObject(item))
                                    }
                                }
                            }
                        }
                    }
                }
                gridsListView.currentIndex = currentIndexWas

                mainWindow.saveStacks(setting)
            }

            function itemMoved(item, srcPos, destPos) {
                var map = iconPositions
                if (map === undefined)
                    map = new Object

                for (var i in map) {
                    var pos = map[i]
                    if (destPos < srcPos && destPos !== -1) {
                        if (pos >= destPos && pos < srcPos)
                            map[i] = pos + 1
                    }
                    else {
                        if (pos > srcPos && (pos <= destPos || destPos === -1))
                            map[i] = Math.max(-1, pos - 1)
                    }
                }
                map[item] = destPos

                //console.log("MOVED: " + item + " from " + srcPos  + " to " + destPos)

//                for (var j in map) {
//                    console.log("> " + j + " " + map[j])
//                }

                iconPositions = map
                saveIconPositions()
            }

            function saveIconPositions() {
                mainWindow.saveIconPositions(iconPositions)
            }

            function moveItemInArray(array, pos1, pos2) {
                // local variables
                var i, tmp;
                // cast input parameters to integers
                pos1 = parseInt(pos1, 10);
                pos2 = parseInt(pos2, 10);
                // if positions are different and inside array
                if (pos1 !== pos2 &&
                    0 <= pos1 && pos1 <= array.length &&
                    0 <= pos2 && pos2 <= array.length) {
                    // save element from position 1
                    tmp = array[pos1];
                    // move element down and shift other elements up
                    if (pos1 < pos2) {
                        for (i = pos1; i < pos2; i++) {
                            array[i] = array[i + 1];
                        }
                    }
                    // move element up and shift other elements down
                    else {
                        for (i = pos1; i > pos2; i--) {
                            array[i] = array[i - 1];
                        }
                    }
                    // put element from position 1 to destination
                    array[pos2] = tmp;
                }
            }

            // Sorry for the copypaste - had no time to do it proper way(
            function stackItemInItem(list, indexStackingTo, indexDragging) {
                //console.log("----------------- STACKING " + indexDragging + " to " + indexStackingTo)

                var itemDragging = list[indexDragging]
                var itemStackingTo = list[indexStackingTo]

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
                //console.log(" L Adding " + itemDragging.caption)
                stackArray.push(root.cloneObject(itemDragging))

                if (stackArray.length === 2) // First time stacking
                    list[indexStackingTo].caption = itemStackingTo.caption + " Group"

                list[indexStackingTo].imagePath = "image://generalicon/stacked/" + itemStackingTo.imagePath.slice(28) + "|" + itemDragging.imagePath.slice(28)
                list[indexStackingTo].stack = stackArray

                //console.log("image: " + list[indexStackingTo].imagePath)
                //console.log("stack: " + list[indexStackingTo].stack)

                return true
            }


            function fetchApps() {
                var appsList = []


                for (var ds in dataSources) {
                    if (dataSources[ds].groupName === "Apps") {

                        // iconPositionsArr is needed to sort moved icons array by position
                        var iconPositionsArr = []
                        for (var iconCaption in iconPositions) {
                            iconPositionsArr.push({"pos": iconPositions[iconCaption], "caption": iconCaption})
                        }

                        iconPositionsArr.sort(function (a,b) {
                            return a.pos - b.pos;
                        })

//                        for (var icon in iconPositionsArr) {
//                            console.log(iconPositionsArr[icon].pos + " - " + iconPositionsArr[icon].caption)
//                        }


                        // console.log("iconPositionsArr ------------")
                        // for (var aa in iconPositionsArr)
                        //     console.log(iconPositionsArr[aa] + " " + aa)
                        // console.log("</iconPositionsArr ------------")

                        var appsPositionsDict = {}

                        // Filling appsList with available apps
                        var appsCount = dataSources[ds].dataSource.getItemCount("Apps")

                        for (var appIndex = 0; appIndex < appsCount; appIndex++) {

                            var newItem = dataSources[ds].dataSource.getContent(appIndex, "Apps")
                            //console.log("++ " + "Apps" + "[" + appIndex + "] - " + newItem.caption + " / " + newItem.id)
                            appsList.push(newItem)

                            if (iconPositions[newItem.caption] !== undefined)
                                appsPositionsDict[newItem.caption] = appsList.length - 1

                            //gridWithGroup.gridView.newItemData(newItem)
                        }

                        // Loading stacks
                        loadStacks(appsList, mainWindow.loadStacks()) // FIXME: do mainWindow.loadStacks() only once

                        // Moving items that are about to be moved to the end // FIXME: too slow and stupid solution
                        var appsShrinkableCount = appsCount
                        for (appIndex = 0; appIndex < appsShrinkableCount; appIndex++)
                            if (iconPositions[appsList[appIndex].caption] !== undefined) {
                                moveItemInArray(appsList, appIndex, appsList.length - 1)
                                //console.log("move to end " + appsList[appsList.length - 1].caption)
                                appsShrinkableCount--
                                appIndex--
                            }
                        //console.log("---")

                        // Reordering loaded items
                        for (var newIndex in iconPositionsArr) {
                            for (appIndex = 0; appIndex < appsCount; appIndex++)
                                if (appsList[appIndex].caption === iconPositionsArr[newIndex].caption) {
                                    if (iconPositionsArr[newIndex].pos === -1)
                                        moveItemInArray(appsList, appIndex, appsList.length - 1)
                                    else
                                        moveItemInArray(appsList, appIndex, iconPositionsArr[newIndex].pos)
                                    break
                                }
                        }

                        // Logging
//                        console.log("--------- REORDERED AND STACKED TO:")
//                        for (appIndex = 0; appIndex < appsCount; appIndex++) {
//                            console.log("++ " + "Apps" + "[" + appIndex + "] - " + appsList[appIndex].caption + " / " + appsList[appIndex].id)
//                            if (appsList[appIndex].stack !== undefined)
//                                for (var ii in appsList[appIndex].stack)
//                                    console.log("   L " + appsList[appIndex].stack[ii].caption)
//                        }

                        break
                    }
                }

                return appsList
            }

            function fetchItemsFromDataSources() {
                var appsList = fetchApps()
                var dataSourcesVar = dataSources

                // Iterating by all GridWithGroupContainers
                for (var currentView = 0; currentView < gridsListView.count; currentView++) {
                    gridsListView.currentIndex = currentView
                    //console.log("iterating gridsListView")

                    if (gridsListView.currentItem) {
                        var childs = gridsListView.currentItem.children

                        // Iterating by it's GridWithGroups
                        for (var child = 0; child < childs.length; child++) {
                            var gridWithGroup = childs[child]
                            //console.log("-== NEW GRID WITH GROUP ==-")
                            if ('gridView' in gridWithGroup)
                            {
                                var gridMaxCount = gridWithGroup.maxCount

                                //console.log("Adding items to " + gridWithGroup.groupName + "; dataSourcesVar.length: " + dataSourcesVar.length)
                                for (var ds = 0; ds < dataSourcesVar.length; ds++) {
                                    //console.log("checking " + dataSourcesVar[ds].groupName + " == " + gridWithGroup.groupName)
                                    if (dataSourcesVar[ds].groupName === gridWithGroup.groupName) {
                                        //console.log("Adding to " + gridWithGroup.groupName)
                                        var itemCount = dataSourcesVar[ds].dataSource.getItemCount(gridWithGroup.groupName)
                                        //console.log("Item count: " + itemCount)
                                        if (dataSourcesVar[ds].index === undefined)
                                            dataSourcesVar[ds].index = 0
                                        //console.log("dataSourcesVar[ds].index: " + dataSourcesVar[ds].index)
                                        for (; dataSourcesVar[ds].index < itemCount &&
                                             (gridWithGroup.maxCount === -1 || gridWithGroup.gridView.count < gridMaxCount ); dataSourcesVar[ds].index++) {

                                            if (gridWithGroup.groupName === "Apps") {
                                                var itemToAdd = appsList[dataSourcesVar[ds].index]

                                                //console.log("++ " + gridWithGroup.groupName + "[" + dataSourcesVar[ds].index + "] - " + itemToAdd.caption + " / " + itemToAdd.id)

                                                // This is needed to prevent ListModel.append from converting JsObject to ListModel
                                                var stack = itemToAdd.stack
                                                itemToAdd.stack = undefined
                                                if (itemToAdd.caption)
                                                    gridWithGroup.gridView.newItemData(itemToAdd)
                                                if (stack !== undefined)
                                                    gridWithGroup.gridView.model.setProperty(gridWithGroup.gridView.model.count - 1, "stack", stack)
                                            }
                                            else
                                                gridWithGroup.gridView.newItemData(dataSourcesVar[ds].dataSource.getContent(dataSourcesVar[ds].index, gridWithGroup.groupName))
                                        }
                                        break
                                    }
                                }
                            }


                        }
                    }
                    gridsListView.currentItem.gridsConnectionChanged()
                }

                gridsListView.currentIndex = 0
            }

            function updateGridsContent() {
                gridsListModel.clear()
                createTabsFromGroups()
                fetchItemsFromDataSources()
            }

            function groupNameChanged(newName) {
                var itemWas = activeGridView.model.get(popupFrame.stackedIconIndex)
                if (popupFrame.stackedIconIndex !== -1 && itemWas !== undefined) {
                    var nameWas = itemWas.caption
                    activeGridView.model.setProperty(popupFrame.stackedIconIndex, "caption", newName)
                    //console.log("updated group from [" + nameWas + "] to [" + newName + "]")

                    if (iconPositions[nameWas] !== undefined) {
                        //console.log("updating movage from [" + nameWas + "] to [" + newName + "]")
                        var iconPositionsArr = iconPositions
                        iconPositionsArr[newName] = iconPositionsArr[nameWas]
                        delete iconPositionsArr[nameWas]
                        iconPositions = iconPositionsArr
                    }
                }
            }

            Component.onCompleted: {
                var dataSourcesDict = new Object
                //console.log("Component.onCompleted: {")
                for (var i = 0; i < groups.length; i++)
                {
                    //console.log("  groups[i].groupName: " + groups[i].groupName)
                    if (dataSourcesDict[groups[i].groupName] === undefined) {
                        //console.log("  not found in dict. Adding: " + groups[i].groupName)

                        groups[i].dataSource.qmlGroupName = groups[i].groupName
                        groups[i].dataSource.resetContent.connect(updateGridsContent)

                        dataSourcesDict[groups[i].groupName] = groups[i]
                    }
                }

                var dataSourcesArr = new Array
                for (var ds in dataSourcesDict)
                    dataSourcesArr.push(dataSourcesDict[ds])
                dataSources = dataSourcesArr

                //console.log("}")
            }

            function hideGroup(isNullifyStackedIndex)
            {
                gridsListView.dndStateChanged(false)

                popupFrame.state = "CLOSED"
                stackCellOpenedId = -1
                topBar.forceActiveFocus()
                activeGridView.myActiveFocus = true
                gridsListView.saveStacks()
                gridsListView.saveIconPositions()

                if (isNullifyStackedIndex !== false)
                    popupFrame.stackedIconIndex = -1
            }

            function showGroup(index, item, iconCoords)
            {
                if (popupFrame.state == "CLOSED")
                {
                    gridsListView.dndStateChanged(true)

                    popupFrame.arrowX = iconCoords.x
                    popupFrame.y = iconCoords.y
                    popupFrame.groupTitle = item.caption

                    popupFrame.gridGroup.gridView.model.clear()
                    popupFrame.stackedIconIndex = index

                    //console.log("stacked group we are opening has length:" + item.stack + " of " + item.stack.length)
                    for (var i = 0; i < item.stack.length; i++)
                    {
                        //console.log("N" + i + ": " + item.stack[i].caption + " [" + item.stack[i].id + "]")
                        popupFrame.gridGroup.gridView.newItemData(item.stack[i])
                    }
                    popupFrame.gridGroup.gridView.forceMyFocus()

                    stackCellOpenedId = item.id

                    popupFrame.state = "OPEN"
                    activeGridView.myActiveFocus = false
                }
                else
                    hideGroup()
            }

            transitions: Transition {
                NumberAnimation { properties: "opacity"; duration: 400 }
            }

            GridWithGroupTabMouse {
                id: gridMouseArea
            }
        }



        // Scroll using mouse wheel
        WheelArea {
            id: wheelArea
            anchors.fill: parent

            onScrollVert: {
                if (popupFrame.state === "CLOSED") {
                    if (gridMouseArea.pressed && gridMouseArea.dndSrcId != -1) {
                        gridMouseArea.dragIconToPrevNextTab(delta <= 0)
                    }
                    else {
                        if (delta > 0)
                            gridsListView.decrementCurrentIndex()
                        else
                            gridsListView.incrementCurrentIndex()
                    }
                }
            }
        }


        // Bottom dots for displaying current tab index
        Row {
            width: childrenRect.width
            height: 30
            anchors {
                top: gridsListView.bottom;
                horizontalCenter: gridsListView.horizontalCenter
            }
            spacing: 20
            visible: gridsVisualModel.count > 1

            Repeater {
                model: gridsVisualModel.count

                Rectangle {
                    width: 6
                    height: 6
                    radius: 3
                    color: gridsListView.currentIndex == index ? "white" : "gray"

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }

                    MouseArea {
                        id: mouse
                        width: 26
                        height: 30
                        anchors.centerIn: parent
                        onClicked: gridsListView.currentIndex = index
                    }
                }
            }
        }

        // Background darkener for displaying popup grid with stacked icons
        Rectangle {
            anchors.top: parent.top
            anchors.topMargin: -15
            width: parent.width
            height: parent.height + 28

            color: Qt.rgba(0, 0, 0, 0.6)
            opacity: popupFrame.state == "OPEN"

            Behavior on opacity {
                NumberAnimation { duration: 200; /*easing.type: Easing.OutQuint*/ }
            }
        }

        // Popup grid with stacked icons
        PopupFrame {
            id: popupFrame

            property int slideHeight: 0
            property int stackedIconIndex: -1

            //anchors.top: parent.bottom
            width: parent.width
            z: 1

            Component.onCompleted: {
                gridGroup.groupNameChanged.connect(gridsListView.groupNameChanged)
            }

            state: "CLOSED"

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
                        slideHeight: y + height < tabRoot.height ? 0 : -(tabRoot.height - (y + height))
                    }
                }
            ]

            transitions: Transition {
                NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
            }
        }
    }
}
