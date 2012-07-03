import QtQuick 1.0

GridView {
    id: grid
    property int columns: 7
    property variant prevGrid
    property variant nextGrid

    cellWidth: width / columns - 1;
    cellHeight: 200
    clip: true

    highlight: Rectangle {
        id: gridSelection
        width: 120
        height: 140
        color: "#c8b0c4de"
        radius: 5
        opacity: 0

        Behavior on opacity {
                 NumberAnimation { duration: 150 }
             }
    }

    highlightMoveDuration: 150

    onActiveFocusChanged: {
        if (highlightItem) // we are not empty and we have selection rectangle
        {
            if (!activeFocus)
                highlightItem.opacity = 0
            else
            {
                highlightItem.opacity = 1
                parent.contentY = highlightItem.y

            }
            console.log("FOCUS AND ACTIVE ITEM here ci: " + currentIndex + "; vis: " + highlightItem.visible)
        }
        else // we are probably empty
        {
            if (activeFocus && nextGrid)
            {
                nextGrid.forceActiveFocus()
                nextGrid.focus = true
                console.log("FORCING FOCUS TO NEXT WITH")
            }
            else
                console.log("nothing here")
        }
    }

    delegate: Button {}

    Keys.onPressed: {
        switch (event.key)
        {
        case Qt.Key_Left:
            if (currentIndex == 0 && prevGrid)
            {
                prevGrid.highlightMoveDuration = 1
                prevGrid.currentIndex = prevGrid.count - 1
                prevGrid.focus = true
                prevGrid.highlightMoveDuration = 150
            }

            moveCurrentIndexLeft()
            event.accepted = true
            break
        case Qt.Key_Right:
            if (currentIndex == count - 1 && nextGrid)
            {
                nextGrid.highlightMoveDuration = 1
                nextGrid.currentIndex = 0
                nextGrid.focus = true
                nextGrid.highlightMoveDuration = 150
            }

            moveCurrentIndexRight()
            event.accepted = true
            break
        case Qt.Key_Up:
            if (currentIndex < columns && prevGrid)
            {
                var roundCount = Math.floor((prevGrid.count) / columns) * columns
                var newCur = (currentIndex % columns) + roundCount - columns * Math.floor((currentIndex % columns) / (prevGrid.count - roundCount))
                console.log(newCur)

                prevGrid.highlightMoveDuration = 1
                prevGrid.currentIndex = newCur
                prevGrid.focus = true
                prevGrid.highlightMoveDuration = 150
            }

            moveCurrentIndexUp()
            event.accepted = true
            break
        case Qt.Key_Down:
            if (currentIndex >= count - columns && nextGrid)
            {
                nextGrid.highlightMoveDuration = 1
                nextGrid.currentIndex = currentIndex % columns
                nextGrid.focus = true
                nextGrid.highlightMoveDuration = 150
            }

            moveCurrentIndexDown()
            event.accepted = true
            break
        }
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
        anchors.fill: parent
        hoverEnabled: true

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
            if (!grid.moving)
            {
                var newCurrentIndex = getItemUnderCursor()
                if (newCurrentIndex != -1)
                    grid.currentIndex = newCurrentIndex
            }
        }

        onClicked: {
            if (!grid.moving)
            {
                var indexClicked = getItemUnderCursor(true)
                model.itemClicked(indexClicked)
            }
        }

        property int index: grid.indexAt(mouseX, mouseY)
    }
}

