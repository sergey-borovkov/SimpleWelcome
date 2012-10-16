import QtQuick 1.1

Column {
    id: gridsContainer

    property variant defaultGroupData
    property variant activeGridGroup
    property variant prevGridGroup: activeGridGroup.prevGridGroup
    property variant nextGridGroup: activeGridGroup.nextGridGroup

    signal gridCurrentItemChanged(variant newCurrentItem)
    property int currentIndex: -1

    // constants
    property int gridContainersSpacing: constants.gridWithGroupsSpacing


    width: parent.width
    height: childrenRect.height
    anchors.bottom: parent.bottom
    spacing: gridContainersSpacing


    function gridsConnectionChanged()
    {
        var visibleGrids = []
        var i

        for (i = 0; i < children.length; i++)
            if ('count' in children[i])
            {
                if (children[i].count)
                    visibleGrids.push(i)
                else
                {
                    children[i].prevGridGroup = children[i]
                    children[i].nextGridGroup = children[i]
                }
            }


        for (i = 0; i < visibleGrids.length; i++)
        {
            var prevIndex = i - 1 < 0 ? visibleGrids.length - 1 : i - 1
            var nextIndex = i + 1 >= visibleGrids.length ? 0 : i + 1

            children[visibleGrids[i]].prevGridGroup = children[visibleGrids[prevIndex]].gridView
            children[visibleGrids[i]].nextGridGroup = children[visibleGrids[nextIndex]].gridView
        }

        if (visibleGrids.length)
        {
            activeGridGroup = children[visibleGrids[0]]
            activeGridGroup.gridView.forceMyFocus()
            topBar.forceActiveFocus()
        }
    }

    function addGridGroup(groupData)
    {
        var prevGridGroup = activeGridGroup
        var groupComponent = Qt.createComponent("GridWithGroup.qml");
        //console.log("ADDING GRID WITH GROUP OF " + groupData)

        groupData['containerIndex'] = children.length

        var groupObject = groupComponent.createObject(gridsContainer, groupData);
        activeGridGroup = groupObject

        activeGridGroup.gridItemCountChanged.connect(gridsConnectionChanged)
        activeGridGroup.gridCurrentItemChanged.connect(gridCurrentItemChanged)
        activeGridGroup.gridMyFocusChanged.connect(gridMyFocusChanged)

        gridsConnectionChanged()
    }

    function gridMyFocusChanged(index) {
        if (index !== -1)
            currentIndex = index

        for (var i = 0; i < children.length; i++)
            if ('gridView' in children[i]) {
                if (children[i].containerIndex === index) {
                    children[i].myActiveFocus = true
                    //console.log("CHANGED IN " + children[i].groupName + " TO true")
                }
                else {
                    children[i].myActiveFocus = false
                    //console.log("CHANGED IN " + children[i].groupName + " TO false")
                }
            }
    }

    function getActiveItem() {
        if (currentIndex != -1)
            for (var i = 0; i < children.length; i++)
                if ('gridView' in children[i] && children[i].containerIndex == currentIndex)
                    return children[i]
        return undefined
    }

    function processKeyboard(key) {
        var activeItem = getActiveItem()
        if (activeItem !== undefined)
            activeItem.gridView.processKeyboard(key)
    }


    Component.onCompleted: {
        if (defaultGroupData !== undefined)
            addGridGroup(defaultGroupData)
        gridsConnectionChanged()
    }
}
