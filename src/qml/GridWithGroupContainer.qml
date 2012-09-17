import QtQuick 1.1

Column {
    id: gridsContainer

    property variant defaultGroupData
    property variant activeGridGroup
    property variant prevGridGroup: activeGridGroup.prevGridGroup
    property variant nextGridGroup: activeGridGroup.nextGridGroup

    signal gridCurrentItemChanged(variant newCurrentItem)

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
            activeGridGroup.gridView.forceActiveFocus()
            topBar.forceActiveFocus()
        }
    }

    function addGridGroup(groupData)
    {
        var prevGridGroup = activeGridGroup
        var groupComponent = Qt.createComponent("GridWithGroup.qml");
        //console.log("ADDING GRID WITH GROUP OF " + groupData)
        var groupObject = groupComponent.createObject(gridsContainer, groupData);
        activeGridGroup = groupObject

//        if (prevGridGroup)
//        {
//            activeGridGroup.prevGridGroup = prevGridGroup.gridView
//            activeGridGroup.nextGridGroup = prevGridGroup.gridView
//            prevGridGroup.prevGridGroup = activeGridGroup.gridView
//            prevGridGroup.nextGridGroup = activeGridGroup.gridView
//        }

        gridsConnectionChanged()
    }

    Component.onCompleted: {
        for (var i = 0; i < children.length; i++)
        {
            if ('gridItemCountChanged' in children[i])
                children[i].gridItemCountChanged.connect(gridsConnectionChanged)

            if ('gridCurrentItemChanged' in children[i])
                children[i].gridCurrentItemChanged.connect(gridCurrentItemChanged)
        }

        if (defaultGroupData !== undefined)
        {
            //console.log("Constructing GridWithGroupContainer for: " + group.group + "; ds: " + group.dataSource + "; start: " + group.startIndex + "; " + group.endIndex)
            addGridGroup(defaultGroupData)
        }
        gridsConnectionChanged()
    }
}
