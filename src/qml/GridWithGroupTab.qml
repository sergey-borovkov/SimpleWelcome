import QtQuick 1.1
import Private 0.1

Item {
    id: tabRoot
    anchors.fill: parent
    clip:true

    property variant groups
    property bool isForceOnOneScreen: false
    property alias tabListView: gridsListView

    function updateGridsContent()
    {
        gridsListView.updateGridsContent()
    }

    Item {
        width: parent.width
        height: parent.height + popupFrame.slideHeight
        anchors.top: parent.top
        anchors.topMargin: -popupFrame.slideHeight

        ListView {
            id: gridsListView


            anchors.top: parent.top
            anchors.topMargin: 16
            width: parent.width
            height: parent.height - 32

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

            function dndStateChanged(isDrag)
            {
                interactive = !isDrag
            }

            function insertGrid(groupData, isOnNewTab)
            {
                var newGridGroup
                if (isOnNewTab)
                {
                    gridsListModel.append( { defaultGroup: groupData } )
                    gridsListView.currentIndex = count - 1
                }
                else
                {
                    gridsListView.currentItem.addGridGroup(groupData)
                }

                if (gridsListView.currentItem)
                {
                    gridsListView.currentItem.activeGridGroup.gridView.dndStateChanged.connect(dndStateChanged)
                    if (gridsListView.currentItem.activeGridGroup.gridView.draggable)
                        gridsListView.currentItem.activeGridGroup.showPopupGroup.connect(showGroup)
                }
            }


            function createTabsFromGroups()
            {
                // Constants. Used hack to retrieve them from C++, no way to do it straightforward AFAIK
                var spacing = constants.gridWithGroupsSpacing, // spacing between GridWithGroups
                        columns = constants.gridColumns,
                        cellRealHeight = constants.cellHeight

                var availableHeight = 0
                //console.log("Starting to split " + availableHeight + "px to grids")
                for (var i = 0; i < groups.length; i++) // Iterating by grids
                {
                    //console.log(i + " - NEW ITERATION!!!!!!!!! with " + groups[i].groupName)
                    var textHeight = (groups[i].groupNameVisible !== undefined && groups[i].groupNameVisible === false ? 0 : constants.groupTextHeight + constants.textToGridSpacing),
                            itemCount = groups[i].dataSource.getItemCount(groups[i].groupName),
                            projectedGroupHeight = textHeight + Math.ceil(itemCount / columns) * cellRealHeight,
                            currentGroup = groups[i]
                    //console.log(i + " - Projected group height: " + projectedGroupHeight + "; Left here: " + availableHeight + ";  itemCount: " + itemCount)

                    if (!itemCount) // No need to add an empty group
                        continue

                    if (projectedGroupHeight < availableHeight || (isForceOnOneScreen && count)) // Grid can be fully placed on the tab
                    {
                        //console.log(i + " - " + groups[i].dataSource + " is fitting the same screen");
                        currentGroup['startIndex'] = 0
                        currentGroup['endIndex'] = itemCount - 1
                        insertGrid(currentGroup, false)
                        availableHeight -= projectedGroupHeight + spacing
                        //console.log(i + " - " + availableHeight + "px left");
                    }
                    else // Grid should be split or created new tab or both
                    {
                        //console.log(i + " - " + groups[i].groupName + " is going to be split or created new tab or both")
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
                                //console.log(i + " - " + "Tab [" + (gridsListView.count - 1) +  "] has no space left. Creating new tab")
                                availableHeight = gridsListView.height - textHeight
                                rowsFit = Math.min(rowsLeftToFit, Math.max(1, Math.floor(availableHeight / cellRealHeight)) ) // We definitely want to have at least one row on newly created tab
                                //console.log(i + " - availableHeight: " + availableHeight + "; cellRealHeight: " + cellRealHeight)
                                currentGroup['startIndex'] = lastNotInsertedItem
                                currentGroup['endIndex'] = Math.min(itemCount - 1, lastNotInsertedItem + rowsFit * columns - 1)
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
                                currentGroup['endIndex'] = Math.min(itemCount - 1, lastNotInsertedItem + rowsFit * columns - 1)
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
                    groups[i].dataSource.resetContent.connect(updateGridsContent)
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

            function showGroup(groupData, groupTitle, iconCoords)
            {
                if (popupFrame.state == "CLOSED")
                {
                    popupFrame.arrowX = iconCoords.x
                    popupFrame.y = iconCoords.y
                    popupFrame.groupTitle = groupTitle

                    popupFrame.gridGroup.gridView.model.clear()
                    popupFrame.gridGroup.startIndex = groupData[0].id
                    popupFrame.gridGroup.endIndex = groupData[0].id + groupData.length - 1

                    //for (var groupData in groupDataArray)
                    //console.log("DATQA :" + groupData.length)
                    for (var i = 0; i < groupData.length; i++)
                        popupFrame.gridGroup.gridView.newItemData(groupData[i].imagePath, groupData[i].caption, groupData[i].id)

                    /*

                dataSource.newItemData.connect(newItemData)
                dataSource.resetContent.connect(resetContent)
                dataSource.getContent()*/

                    popupFrame.state = "OPEN"
                }
                else
                    popupFrame.state = "CLOSED"
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
            onScrollHorz: _scroll(delta)

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
                bottom: gridsListView.bottom;
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

                onClicked: {
                    popupFrame.state = "CLOSED"
                }
            }

            Behavior on opacity {
                NumberAnimation { duration: 200; /*easing.type: Easing.OutQuint*/ }
            }
        }

        // Popup grid with stacked icons
        PopupFrame {
            id: popupFrame

            property int slideHeight: 0

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
