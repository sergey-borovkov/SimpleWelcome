import QtQuick 1.0

GridView {
    id: grid
    property variant dataSource
    property variant prevGrid
    property variant nextGrid
    property int startIndex
    property int endIndex
    property bool draggable: false

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
        appsModel.append( { imagePath: iconPath, caption: name, id: itemId, pinned: undefined })
    }

    function resetContent() {
        if(typeof model !== 'undefined')
            model.clear()
    }

    function onItemClicked(newIndex) {
        dataSource.itemClicked(newIndex == -1 ? newIndex : appsModel.get(newIndex).id)
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

        function getItemUnderCursor(isForceRecheck)
        {
            var wasCurrentIndex = grid.currentIndex
            var mouseXReal = mouseX + grid.contentX, mouseYReal = mouseY + grid.contentY
            var wasContentX = grid.contentX, wasContentY = grid.contentY
            var indexUnderMouse = grid.indexAt(mouseXReal, mouseYReal)
            var result = -1

            if (indexUnderMouse != -1 && (grid.currentIndex != indexUnderMouse || isForceRecheck))
            {
                // Dirty hack to check if there is a need to change the current item after mouse position have changed
                grid.currentIndex = indexUnderMouse
                if (grid.currentItem && grid.currentItem.x < mouseXReal && grid.currentItem.y < mouseYReal &&
                      grid.currentItem.x + grid.currentItem.width > mouseXReal && grid.currentItem.y + grid.currentItem.height > mouseYReal)
                {
                    result = indexUnderMouse
                }
                grid.currentIndex = wasCurrentIndex
                grid.contentX = wasContentX
                grid.contentY = wasContentY
            }
            return result
        }


        onMousePositionChanged: {
            if (!grid.moving && dndSrcId == -1)
            {
                // Optimize later to lesser use of getItemUnderCursor(true)
                var newCurrentIndex = getItemUnderCursor(!grid.activeFocus)
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
            else
            {
                var index = getItemUnderCursor(true)
                if (dndSrcId != -1 && index != -1 && index != dndDest)
                {
                    dndDestId = model.get(index).id
                    model.move(dndDest, index, 1)
                    dndDest = index
                }
            }
        }

        onPressed: {
            pressedOnIndex = getItemUnderCursor(true)
        }

        onPressAndHold: {
            if (draggable)
            {
                var index = getItemUnderCursor(true)
                if (index != -1 && pressedOnIndex == index)
                {
                    dndDest = index
                    dndSrc = index
                    dndSrcId = model.get(dndSrc).id
                    dndDestId = dndSrcId
                    //console.log("dndSrc, dndSrcId, dndDest, dndDestId: " + dndSrc + " " + dndSrcId + " " + dndDest + " " + dndDestId)
                    dndStateChanged(true)

                    //console.log("NOW----------------")
                    //for (var i = 0; i < model.count; i++)
                    //    console.log(model.get(i).caption + " | " + model.get(i).id + " | " + i)
                    //console.log("END----------------")
                }
            }

        }
        onReleased: {
            var dndSrcIdSaved = dndSrcId
            dndSrcId = -1

            if (dndSrcIdSaved != -1)
            {
                dndStateChanged(false)

                if (typeof dataSource.itemDragged !== "undefined" && dndDestId != -1)
                {
                    //console.log("dndDestId: " + dndDestId)
                    dataSource.itemDragged(dndSrcIdSaved, dndDestId)

                    console.log("dndSrc, dndSrcIdStaged, dndDest, dndDestId: " + dndSrc + " " + dndSrcIdSaved + " " + dndDest + " " + dndDestId)
                    model.set(dndDest, {"id": dndDestId})
                    console.log("For dragged item we set src id: " + dndDestId)
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

        onClicked: {
            if (!grid.moving)
            {
                var indexClicked = getItemUnderCursor(true)
                model.itemClicked(indexClicked)
            }
        }
    }
}

