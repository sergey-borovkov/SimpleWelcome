import QtQuick 1.1

Item {
    //anchors.fill: parent
    width: parent.width
    height: parent.height
    anchors.top: parent.top
    anchors.topMargin: popupFrame.open ? -200 : 0

    property variant groups

    ListView {
        id: gridsListView

        anchors.fill: parent
        snapMode: ListView.SnapOneItem
        orientation: ListView.Horizontal
        flickDeceleration: 5000
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 240
        cacheBuffer: 20000
        highlightRangeMode: ListView.StrictlyEnforceRange

        model: VisualDataModel {
            id: gridsVisualModel

            model: ListModel {
                id: gridsListModel
            }

            delegate: GridWithGroupContainer {
                width: gridsListView.width
                height: gridsListView.height
                groups: groupsList
            }
        }

        /*onCurrentIndexChanged: {
        if (currentItem && currentItem.grid)
            currentItem.grid.forceActiveFocus()
        topBar.forceActiveFocus()
    }

    function currentTabIndexChanged(newCurrentIndex) {
        tabListView.currentIndex = newCurrentIndex
    }

    Component.onCompleted: {
        searchGridModel.currentTabIndexChanged.connect(currentTabIndexChanged)

        gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)
    }*/

        function gridsSelectionChanged(obj) // Used to be used for vertical scrolling
        {
            if (moving)
                return

            var itemHere = mapFromItem(obj, 0, 0)
            var itemInContainer = mapToItem(gridsContainer, itemHere.x, itemHere.y)

            if (itemInContainer.y + constants.cellHeight > height + contentY)
                contentY += itemHere.y - height + constants.cellHeight
            else if(itemInContainer.y < contentY)
            {
                if (itemInContainer.y < constants.cellHeight)
                    contentY = 0
                else
                    contentY += itemHere.y
            }
        }

        function showPopup()
        {
            popupFrame.open = !popupFrame.open
            //console.log("popup")
        }

        function insertGrid(groupData, isOnNewTab)
        {
            var newGridGroup
            if (isOnNewTab)
            {
                gridsListModel.append( { groupsList: groupData } )
                gridsListView.currentIndex = count - 1
            }
            else
            {
                gridsListView.currentItem.addGridGroup(groupData.group, groupData.dataSource, groupData.startIndex, groupData.endIndex)
            }

            gridsListView.currentItem.activeGridView.model.itemClicked.connect(showPopup)

            //appsModel.itemClicked.connect()
        }


        function createTabsFromGroups()
        {
            // Constants. Used hack to retrieve them from C++, no way to do it straightforward AFAIK
            var textHeight = constants.groupTextHeight + constants.textToGridSpacing,
                    spacing = constants.gridWithGroupsSpacing, // spacing between GridWithGroups
                    columns = constants.gridColumns,
                    cellRealHeight = constants.cellHeight

            var availableHeight = 0
            //console.log("Starting to split " + availableHeight + "px to grids")
            for (var i = 0; i < groups.length; i++) // Iterating by grids
            {
                //console.log(i + " - NEW ITERATION!!!!!!!!!")
                var itemCount = groups[i].dataSource.getItemCount(groups[i].group)
                var projectedGroupHeight = textHeight + Math.ceil(itemCount / columns) * cellRealHeight
                //console.log(i + " - Projected group height: " + projectedGroupHeight)

                if (projectedGroupHeight < availableHeight) // Grid can be fully placed on the tab
                {
                    //console.log(i + " - " + groups[i].dataSource + " is fitting the same screen");
                    insertGrid(groups[i], false)
                    availableHeight -= projectedGroupHeight + spacing
                    //console.log(i + " - " + availableHeight + "px left");
                }
                else // Grid should be split or created new tab or both
                {
                    //console.log(i + " - " + groups[i].group + " is going to be split or created new tab or both")
                    var currentGroup = groups[i]
                    var rowsLeftToFit = Math.ceil(itemCount / columns)
                    var lastNotInsertedItem = 0

                    if (rowsLeftToFit <= 0) // REMOVE THIS
                    {
                        //console.log(i + " - " + "Empty group. Left: " + availableHeight)
                        insertGrid(currentGroup, true)
                        availableHeight = gridsListView.height - textHeight - spacing
                        //console.log(i + " - " + "Current: " + gridsListView.currentIndex)
                    }

                    while (rowsLeftToFit > 0)
                    {
                        availableHeight -= textHeight
                        var rowsFit = Math.floor(availableHeight / cellRealHeight)
                        //console.log(i + " - Rows fit currently: " + rowsFit + "; availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight + "; " + (availableHeight / cellRealHeight))
                        if (rowsFit <= 0) // Current tab has no space left. Creating new tab
                        {
                            //console.log(i + " - " + "Current tab has no space left. Creating new tab")
                            availableHeight = gridsListView.height - textHeight
                            rowsFit = Math.max(1, Math.floor(availableHeight / cellRealHeight)) // We definitely want to have at least one row on newly created tab
                            //console.log(i + " - availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight)
                            currentGroup['startIndex'] = lastNotInsertedItem
                            currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                            lastNotInsertedItem += rowsFit * columns
                            rowsLeftToFit -= rowsFit
                            //console.log(i + " - " + "Fitted "  + rowsFit + " rows")

                            //console.log("=== " + currentGroup.startIndex + "; " + currentGroup.endIndex)
                            insertGrid(currentGroup, true)
                            availableHeight -= rowsFit * cellRealHeight + spacing
                        }
                        else // It's still possible to insert some rows to current tab. Inserting
                        {
                            //console.log(i + " - " + "It's still possible to insert some rows to current tab. Inserting " + rowsFit)

                            currentGroup['startIndex'] = lastNotInsertedItem
                            currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                            lastNotInsertedItem += rowsFit * columns
                            rowsLeftToFit -= rowsFit

                            //console.log(i + " - adding to current tab: " + currentGroup.group + ", " + currentGroup.dataSource + " [" + currentGroup.startIndex + " to " + currentGroup.endIndex + "]")
                            insertGrid(currentGroup, false)
                            availableHeight -= rowsFit * cellRealHeight + spacing
                            //console.log(i + " - Taken " + rowsFit * cellRealHeight + "px; " + availableHeight)
                        }

                        //console.log(i + " - ROWS FITTING ITERATION END")
                    }
                }
            }
        }

        Component.onCompleted: {
            //gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)

            for (var i = 0; i < groups.length; i++)
            {
                groups[i].dataSource.dataClear.connect(updateGridsContent)
            }

            createTabsFromGroups()
            gridsListView.currentIndex = 0
        }

        function updateGridsContent() {
            gridsListModel.clear()
            //console.log("Clearing " + groups[0].group)
            createTabsFromGroups()
            gridsListView.currentIndex = 0
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }

    }


    Row {
        width: childrenRect.width
        height: 30
        anchors {
            bottom: gridsListView.bottom;
            horizontalCenter: gridsListView.horizontalCenter
        }
        spacing: 20
        //visible: gridsVisualModel.count > 1

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

    PopupFrame {
        id: popupFrame
        anchors.top: parent.bottom
        height: 260
        width: parent.width
        z: 1
    }

    Behavior on anchors.topMargin {
        NumberAnimation { duration: 600; /*easing.type: Easing.OutQuint*/ }
    }
}
