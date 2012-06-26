import QtQuick 1.0

GridView {
    id: grid
    cellWidth: width / 7 - 1;
    cellHeight: 200
    focus: true
    clip: true

    /*onFocusChanged: {
        if(!focus) // fucking dirty hack to receive focus
            grid.forceActiveFocus()
    }*/

    highlight: Rectangle {
        width: 120
        height: 140
        color: "#c8b0c4de"
        radius: 5
    }
    keyNavigationWraps: true

    delegate: Button {}

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

