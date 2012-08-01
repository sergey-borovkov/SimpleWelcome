import QtQuick 1.1

ListView {
    id: gridsListView

    anchors.fill: parent

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

//        SearchTab {
//            id: searchTab
//            width: tabListView.width
//            height: tabListView.height
//        }

//        WelcomeTab {
//            id: welcomeTab
//            width: tabListView.width
//            height: tabListView.height
//        }

//        ApplicationsTab {
//            id: appsTab
//            width: tabListView.width
//            height: tabListView.height
//        }
    }

    snapMode: ListView.SnapOneItem
    orientation: ListView.Horizontal
    //boundsBehavior: Flickable.StopAtBounds
    flickDeceleration: 5000
    highlightFollowsCurrentItem: true
    highlightMoveDuration: 240
    cacheBuffer: 20000
    //highlightRangeMode: ListView.StrictlyEnforceRange

    //currentIndex: 1
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

    Component.onCompleted: {
        //gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)

        //var tmp = Qt.createComponent("GridWithGroup.qml");
        //var tmp2 = tmp.createObject(gridsListView, { group: "Favorites", dataSource: dataSource_Favorites });

        //newObject.parent = gridsVisualModel
//        console.log(newObject)
        //gridsVisualModel.children.append(newObject)

        var groups = [
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

        var availableHeight = 0
        console.log("Starting to split " + availableHeight + "px to grids")
        for (var i = 0; i < groups.length; i++) // Iterating by grids
        {
            console.log(i + " - NEW ITERATION!!!!!!!!!")
            var itemCount = groups[i].dataSource.getItemCount()
            var textHeight = 24, spacing = 16, columns = 7, cellRealHeight = 200

            var projectedGroupHeight = textHeight + spacing + Math.ceil(itemCount / columns) * cellRealHeight
            if (projectedGroupHeight < availableHeight) // Grid can be fully placed on the tab
            {
                console.log(i + " - " + groups[i].dataSource + " is fitting the same screen");
                availableHeight -= projectedGroupHeight
                gridsListView.currentItem.addGridGroup(groups[i].group, groups[i].dataSource)
                console.log(i + " - " + availableHeight + "px left");
            }
            else // Grid should be split
            {
                console.log(i + " - " + groups[i].group + " is going to be split")
                var currentGroup = groups[i]
                var rowsLeftToFit = Math.ceil(itemCount / columns)
                var lastNotInsertedItem = 0

                if (rowsLeftToFit <= 0)
                {
                    console.log(i + " - " + "Empty group. Left: " + availableHeight)
                    gridsListModel.append( { groupsList: currentGroup } )
                    gridsListView.currentIndex = count - 1
                    console.log(i + " - " + "Current: " + gridsListView.currentIndex)
                    availableHeight = gridsListView.height - (textHeight + spacing)
                }

                while (rowsLeftToFit > 0)
                {
                    availableHeight -= textHeight + spacing
                    var rowsFit = Math.ceil(availableHeight / cellRealHeight)
                    console.log(i + " - Next iteration on " + currentGroup.dataSource + "; lastNotInsertedItem: " + lastNotInsertedItem)
                    if (!rowsFit) // Current tab has no space left. Creating new tab
                    {
                        console.log(i + " - " + "Current tab has no space left. Creating new tab")

                        availableHeight = gridsListView.height - (textHeight + spacing)
                        rowsFit = Math.ceil(availableHeight / cellRealHeight)
                        currentGroup['startIndex'] = lastNotInsertedItem
                        currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                        lastNotInsertedItem += rowsFit * columns
                        rowsLeftToFit -= rowsFit
                        console.log(i + " - " + "Fitted "  + rowsFit + " rows")

                        console.log("=== " + currentGroup.startIndex + "; " + currentGroup.endIndex)
                        gridsListModel.append( { groupsList: currentGroup } )
                        gridsListView.currentIndex = count - 1
                        availableHeight -= rowsFit * cellRealHeight
                    }
                    else // It's still possible to insert some rows to current tab. Inserting
                    {
                        console.log(i + " - " + "It's still possible to insert some rows to current tab. Inserting " + rowsFit)

                        currentGroup['startIndex'] = lastNotInsertedItem
                        currentGroup['endIndex'] = lastNotInsertedItem + rowsFit * columns - 1
                        lastNotInsertedItem += rowsFit * columns
                        rowsLeftToFit -= rowsFit

                        console.log("|")
                        console.log("|")
                        console.log("|")
                        console.log(i + " - adding to current tab: " + currentGroup.group + ", " + currentGroup.dataSource + " [" + currentGroup.startIndex + " to " + currentGroup.endIndex + "]")
                        gridsListView.currentItem.addGridGroup(currentGroup.group, currentGroup.dataSource, currentGroup.startIndex, currentGroup.endIndex)
                        availableHeight -= rowsFit * cellRealHeight
                        console.log(i + " - Taken " + rowsFit * cellRealHeight + "px; " + availableHeight)
                    }


                    console.log(i + " - " + "On " + i + " tab is alailable " + rowsFit + " rows")

                }
            }
        }

//        gridsListModel.append( { groupsList: {group: "Recent Applications", dataSource: dataSource_RecentApps} } )
//        gridsListModel.append( { groupsList: {group: "Recent documents", dataSource: dataSource_Documents} } )
//        gridsListModel.append( { groupsList: {group: "Favorites", dataSource: dataSource_Favorites} } )

        /*console.log("CHANGING CURRENT INDEX")
        var wasCurrent = gridsListView.currentIndex
        gridsListView.currentIndex = 1
        var item0 = gridsListView.currentItem*/
        /*gridsListView.currentIndex = 2
        var item1 = gridsListView.currentItem
        gridsListView.currentIndex = wasCurrent

        item0.prevGridGroup = item1.gridView
        item0.nextGridGroup = item1.gridView
        item1.prevGridGroup = item0.gridView
        item1.nextGridGroup = item0.gridView*/

        /*console.log("TRYING TO ADD FAVS")
        item0.addGridGroup("Favorites", dataSource_Favorites)

        console.log("got: " + item0 + " and " + item1)*/

        //gridsListModel.setProperty(0, "nextGroup", )








        /*model : VisualItemModel {
            id: gridsListModel

            GridWithGroupContainer {
                id: gridsContainer

                GridWithGroup {
                    groupName: "Recent Applications"
                    gridDataSource: dataSource_RecentApps
                    width: tabListView.width
                }

                GridWithGroup {
                    groupName: "Favorites"
                    gridDataSource: dataSource_Favorites
                    width: tabListView.width
                }

                GridWithGroup {
                    groupName: "Recent documents"
                    gridDataSource: dataSource_Documents
                    width: tabListView.width
                }
            }
        }*/





    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }

}


/*Flickable {
    id: flick
    anchors.fill: parent
    contentWidth: parent.width
    contentHeight: gridsContainer.height + 32
    boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
    flickableDirection: Flickable.VerticalFlick

    Behavior on contentY {
        NumberAnimation { duration: 150 }
    }

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

    Component.onCompleted: {
        gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)
    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }
}
*/
