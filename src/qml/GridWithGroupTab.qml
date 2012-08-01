import QtQuick 1.1

ListView {
    id: gridsListView

    property variant groups

    anchors.fill: parent
    snapMode: ListView.SnapOneItem
    orientation: ListView.Horizontal
    flickDeceleration: 5000
    highlightFollowsCurrentItem: true
    highlightMoveDuration: 240
    cacheBuffer: 20000
    //highlightRangeMode: ListView.StrictlyEnforceRange

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


    function gridsSelectionChanged(obj)
    {
        if (moving)
            return

        var itemHere = mapFromItem(obj, 0, 0)
        var itemInContainer = mapToItem(gridsContainer, itemHere.x, itemHere.y)

        if (itemInContainer.y + 200 > height + contentY)
            contentY += itemHere.y - height + 200
        else if(itemInContainer.y < contentY)
        {
            if (itemInContainer.y < 200)
                contentY = 0
            else
                contentY += itemHere.y
        }
    }

    function createTabsFromGroups()
    {
        // Constants. Replace them with the appropriate reference
        var textHeight = 24 + 16, // text height + spacing after it
               spacing = 32, // spacing between GridWithGroups
               columns = 7,
               cellRealHeight = 200


        var availableHeight = 0
        console.log("Starting to split " + availableHeight + "px to grids")
        for (var i = 0; i < groups.length; i++) // Iterating by grids
        {
            console.log(i + " - NEW ITERATION!!!!!!!!!")
            var itemCount = groups[i].dataSource.getItemCount()
            var projectedGroupHeight = textHeight + Math.ceil(itemCount / columns) * cellRealHeight
            console.log(i + " - Projected group height: " + projectedGroupHeight)

            if (projectedGroupHeight < availableHeight) // Grid can be fully placed on the tab
            {
                console.log(i + " - " + groups[i].dataSource + " is fitting the same screen");
                availableHeight -= projectedGroupHeight + spacing
                gridsListView.currentItem.addGridGroup(groups[i].group, groups[i].dataSource)
                console.log(i + " - " + availableHeight + "px left");
            }
            else // Grid should be split or created new tab or both
            {
                console.log(i + " - " + groups[i].group + " is going to be split or created new tab or both")
                var currentGroup = groups[i]
                var rowsLeftToFit = Math.ceil(itemCount / columns)
                var lastNotInsertedItem = 0

                if (rowsLeftToFit <= 0) // REMOVE THIS
                {
                    console.log(i + " - " + "Empty group. Left: " + availableHeight)
                    gridsListModel.append( { groupsList: currentGroup } )
                    gridsListView.currentIndex = count - 1
                    console.log(i + " - " + "Current: " + gridsListView.currentIndex)
                    availableHeight = gridsListView.height - textHeight - spacing
                }

                while (rowsLeftToFit > 0)
                {
                    availableHeight -= textHeight
                    var rowsFit = Math.floor(availableHeight / cellRealHeight)
                    console.log(i + " - Rows fit currently: " + rowsFit + "; availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight + "; " + (availableHeight / cellRealHeight))
                    if (rowsFit <= 0) // Current tab has no space left. Creating new tab
                    {
                        console.log(i + " - " + "Current tab has no space left. Creating new tab")
                        availableHeight = gridsListView.height - textHeight
                        rowsFit = Math.max(1, Math.floor(availableHeight / cellRealHeight)) // We definitely want to have at least one row on newly created tab
                        console.log(i + " - availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight)
                        currentGroup['startIndex'] = lastNotInsertedItem
                        currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                        lastNotInsertedItem += rowsFit * columns
                        rowsLeftToFit -= rowsFit
                        console.log(i + " - " + "Fitted "  + rowsFit + " rows")

                        console.log("=== " + currentGroup.startIndex + "; " + currentGroup.endIndex)
                        gridsListModel.append( { groupsList: currentGroup } )
                        gridsListView.currentIndex = count - 1
                        availableHeight -= rowsFit * cellRealHeight + spacing
                    }
                    else // It's still possible to insert some rows to current tab. Inserting
                    {
                        console.log(i + " - " + "It's still possible to insert some rows to current tab. Inserting " + rowsFit)

                        currentGroup['startIndex'] = lastNotInsertedItem
                        currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                        lastNotInsertedItem += rowsFit * columns
                        rowsLeftToFit -= rowsFit

                        console.log(i + " - adding to current tab: " + currentGroup.group + ", " + currentGroup.dataSource + " [" + currentGroup.startIndex + " to " + currentGroup.endIndex + "]")
                        gridsListView.currentItem.addGridGroup(currentGroup.group, currentGroup.dataSource, currentGroup.startIndex, currentGroup.endIndex)
                        availableHeight -= rowsFit * cellRealHeight + spacing
                        console.log(i + " - Taken " + rowsFit * cellRealHeight + "px; " + availableHeight)
                    }

                    console.log(i + " - ROWS FITTING ITERATION END")
                }
            }
        }
    }

    Component.onCompleted: {
        //gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)

        groups = [
                    {
                        group: "Recent Applications",
                        dataSource: dataSource_RecentApps
                    },
                    {
                        group: "Favorites",
                        dataSource: dataSource_Favorites
                    },
                    {
                        group: "Recent documents",
                        dataSource: dataSource_Documents
                    },
                ]

        createTabsFromGroups()
    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }

}
