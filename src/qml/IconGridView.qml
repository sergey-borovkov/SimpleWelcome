import QtQuick 1.0

GridView {
    id: grid
    property variant dataSource
    property int columns: 7
    property variant prevGrid
    property variant nextGrid
    property int highlightMoveDurationConst: 150
    property int spacing: Math.max(0, (parent.width - 120 * columns) / (columns + 1))
    signal selectionChangedByKeyboard(variant newCurrentItem)

    anchors.leftMargin: spacing
    anchors.left: parent.left

    cellWidth: (width - spacing) / columns - 1
    cellHeight: 200

    delegate: Cell {}

    highlight: Rectangle {
        id: gridSelection
        property int animationDuration: 150
        color: "#c8b0c4de"
        radius: 5
        opacity: 1

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
        appsModel.append( { imagePath: iconPath, caption: name, id: itemId})
    }

    function dataClear() {
        appsModel.clear()
    }

    function onItemClicked(newIndex) {
        dataSource.itemClicked(newIndex == -1 ? newIndex : appsModel.get(newIndex).id)
    }

    Component.onCompleted: {
        dataSource.newItemData.connect(newItemData)
        dataSource.dataClear.connect(dataClear)
        dataSource.updateContent()
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

        property int draggingItemIndex: -1
        property int newIndex
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
            if (!grid.moving && draggingItemIndex == -1)
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
                if (draggingItemIndex != -1 && index != -1 && index != newIndex)
                    model.move(newIndex, newIndex = index, 1)
            }
        }

        onPressed: {
            pressedOnIndex = getItemUnderCursor(true)
        }

        onPressAndHold: {
            var index = getItemUnderCursor(true)
            if (index != -1 && pressedOnIndex == index)
                draggingItemIndex = model.get(newIndex = index).id
        }
        onReleased: {
            draggingItemIndex = -1
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

