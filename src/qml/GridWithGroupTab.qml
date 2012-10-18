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

    function updateGridsContent()
    {
        gridsListView.updateGridsContent()
    }

    function processKeyboard(key) {
        if (popupFrame.state == "OPEN") {
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

        Component.onCompleted: {
            popupFrame.gridGroup.gridView.draggedOut.connect(draggedOut)
        }

        function draggedOut(item) {
            gridsListView.hideGroup()
            //console.log(item.caption + " GOT")
            gridsListView.activeGridView.newItemData(item)
            gridsListView.activeGridView.unstackItemInItem(popupFrame.stackedIconIndex, gridsListView.activeGridView.count - 1)
            gridsListView.activeGridView.startDragging(gridsListView.activeGridView.count - 1)

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

            function insertGrid(groupData, isOnNewTab)
            {
                if (isOnNewTab)
                {
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
                    newGridGroup.gridView.dndStateChanged.connect(dndStateChanged)
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
                        currentGroup['maxCount'] = itemCount - 1
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
                                currentGroup['maxCount'] = Math.min(itemCount - 1 - lastNotInsertedItem, rowsFit * columns - 1)
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

                                currentGroup['maxCount'] = Math.min(itemCount - 1 - lastNotInsertedItem, rowsFit * columns - 1)
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
                //forEachGridGroup("Apps", loadStacks, mainWindow.loadSetting("Stacks"))
            }

            function forEachGridGroup(group, gridGroupProcessFunction, args, containerProcessFunction) {
                var currentIndexWas = gridsListView.currentIndex

loop1:
                // Iterating by all GridWithGroupContainers
                for (var currentView = 0; currentView < gridsListView.count; currentView++) {
                    gridsListView.currentIndex = currentView

                    if (containerProcessFunction !== undefined)
                        containerProcessFunction(gridsListView.currentItem)

                    if (gridsListView.currentItem) {
                        var childs = gridsListView.currentItem.children
loop2:
                        // Iterating by it's GridWithGroups
                        for (var child = 0; child < childs.length; child++) {
                            //console.log("CAME ACROSS:" + childs[child].groupName + " WHEN NEED " + group)
                            if ('count' in childs[child] && (!group || childs[child].groupName === group))
                            {
                                var res = gridGroupProcessFunction(childs[child], args)
                                if (!res) {
                                    //console.log("LOOP BREAKER!")
                                    continue loop1
                                }
                                else
                                    break loop1
                            }
                        }
                    }
                }
                gridsListView.currentIndex = currentIndexWas
            }

            function loadStacks(gridWithGroup, savedStacks) {
                var iconGridView = gridWithGroup.gridView
                //console.log("LOADING STACKS")

                for (var stackName in savedStacks) {

                    var captionsList = savedStacks[stackName].split(",")
                    var captionStackingTo = captionsList[0]
                    //console.log("Object item:", captionStackingTo, "=", captionsList)

                    var model = iconGridView.model
                    var indexStackingTo = -1
                    for (var i = 0; i < model.count; i++) {
                        //console.log("LOADING " + stackName + " checking with " + model.get(i).caption)
                        if (model.get(i).caption === captionStackingTo) {
                            indexStackingTo = i
                            break
                        }
                    }

                    if (indexStackingTo == -1)
                        continue

                    for (var captionToStackIndex in captionsList) {
                        var captionToStack = captionsList[captionToStackIndex]
                        if (captionToStack !== captionStackingTo) {
                            //console.log("NEED TO STACK: " + captionToStack)

                            var indexToStack = -1
                            for (var j = 0; j < model.count; j++)
                                if (model.get(j).caption === captionToStack) {
                                    indexToStack = j
                                    break
                                }

                            if (indexToStack != -1) {
                                iconGridView.stackItemInItem(indexStackingTo, indexToStack, true)
                                //console.log("SETTING " + indexStackingTo + " caption to " + stackName)
                                model.setProperty(indexStackingTo, "caption", stackName)
                                model.remove(indexToStack)
                            }
                        }
                    }

                }

                //console.log("LOADING STACKS FINISHED ---------------- " + iconGridView.count)
            }

            function saveStacks() {
                var setting = []

                forEachGridGroup("Apps", function(gridWithGroup) {
                    var iconGridView = gridWithGroup.gridView
                    var model = iconGridView.model
                    //console.log("SAVING stacking")

                    for (var i = 0; i < model.count; i++) {
                        var item = model.get(i)
                        var stack = item.stack
                        if (stack !== undefined) {
                            setting.push(iconGridView.copyObjectByValue(item))
                            //console.log(stack.length + "; at " + i)
                        }
                    }
                    return true
                })

                mainWindow.saveSetting("Stacks", setting)
            }

            function fetchItemsFromDataSources() {
                var dataSourcesVar = dataSources

                // Iterating by all GridWithGroupContainers
                for (var currentView = 0; currentView < gridsListView.count; currentView++) {
                    gridsListView.currentIndex = currentView
                    gridsListView.currentItem.gridsConnectionChanged()
                    //console.log("iterating gridsListView")

                    if (gridsListView.currentItem) {
                        var childs = gridsListView.currentItem.children

                        // Iterating by it's GridWithGroups
                        for (var child = 0; child < childs.length; child++) {
                            var gridWithGroup = childs[child]
                            //console.log("CAME ACROSS:" + gridWithGroup.groupName)
                            if ('count' in gridWithGroup)
                            {
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
                                             (gridWithGroup.maxCount === -1 || gridWithGroup.gridView.count <= gridWithGroup.maxCount ); dataSourcesVar[ds].index++) {
                                            //console.log("++ " + gridWithGroup.groupName + "[" + dataSourcesVar[ds].index + "]")
                                            gridWithGroup.gridView.newItemData(dataSourcesVar[ds].dataSource.getContent(dataSourcesVar[ds].index, gridWithGroup.groupName))
                                        }
                                        break
                                    }
                                }
                            }
                        }
                    }
                }

                gridsListView.currentIndex = 0
            }

            function updateGridsContent() {
                gridsListModel.clear()
                createTabsFromGroups()
                fetchItemsFromDataSources()
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

            function hideGroup()
            {
                popupFrame.state = "CLOSED"
                stackCellOpenedId = -1
                topBar.forceActiveFocus()
                activeGridView.myActiveFocus = true
                saveStacks()
            }

            function showGroup(index, item, iconCoords)
            {
                if (popupFrame.state == "CLOSED")
                {
                    popupFrame.arrowX = iconCoords.x
                    popupFrame.y = iconCoords.y
                    popupFrame.groupTitle = item.caption

                    popupFrame.gridGroup.gridView.model.clear()
                    popupFrame.stackedIconIndex = index

                    //console.log("stacked group we are opening has length:" + item.stack.length)
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

        }

        // Scroll using mouse wheel
        WheelArea {
            id: wheelArea
            anchors.fill: parent

            onScrollVert: _scroll(delta)

            function _scroll(delta) {
                // See Qt documentation of QGraphicsSceneWheelEvent
                // Most mice report delta = 120
                var pages_delta = Math.round(delta / 120)
                if (pages_delta === 0)
                    pages_delta = (delta > 0 ? 1 : -1)
                gridsListView.currentIndex = ((gridsListView.currentIndex + gridsListView.count - pages_delta) % gridsListView.count)
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

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true

                onClicked: gridsListView.hideGroup()
            }

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
