import QtQuick 1.1

Column {
    id: gridsContainer
    width: parent.width
    height: childrenRect.height
    anchors.bottom: parent.bottom
    spacing: 32

    property variant activeGridView

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

            children[visibleGrids[i]].prevGridGroup = children[visibleGrids[prevIndex]]
            children[visibleGrids[i]].nextGridGroup = children[visibleGrids[nextIndex]]
        }

        if (visibleGrids.length)
        {
            activeGridView = children[visibleGrids[0]].gridView
            activeGridView.forceActiveFocus()
            topBar.forceActiveFocus()
        }
    }

    Component.onCompleted: {
        for (var i = 0; i < children.length; i++)
            if ('gridItemCountChanged' in children[i])
                children[i].gridItemCountChanged.connect(gridsConnectionChanged)

        gridsConnectionChanged()
    }
}
