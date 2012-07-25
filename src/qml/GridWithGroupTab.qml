import QtQuick 1.1

ListView {
    id: gridsListView

    anchors.fill: parent

    model: VisualDataModel {
        id: gridsVisualModel

        model: ListModel {
            id: gridsListModel
        }

        delegate: GridWithGroup {
            groupName: group
            gridDataSource: dataSource
            width: gridsListView.width
            height: gridsListView.height
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

        gridsListModel.append( { group: "Recent Applications", dataSource: dataSource_RecentApps } )
        gridsListModel.append( { group: "Recent documents", dataSource: dataSource_Documents } )
        gridsListModel.append( { group: "Favorites", dataSource: dataSource_Favorites } )


        var wasCurrent = gridsListView.currentIndex
        gridsListView.currentIndex = 1
        var item0 = gridsListView.currentItem
        gridsListView.currentIndex = 2
        var item1 = gridsListView.currentItem
        gridsListView.currentIndex = wasCurrent

        item0.prevGridGroup = item1
        item0.nextGridGroup = item1
        item1.prevGridGroup = item0
        item1.nextGridGroup = item0

        console.log("got: " + item0 + " and " + item1)

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
