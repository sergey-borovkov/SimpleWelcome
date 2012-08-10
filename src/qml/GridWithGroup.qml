import QtQuick 1.1

Column {
    property alias gridView: iconGridView
    property alias groupName: groupLabel.text

    property alias gridDataSource: iconGridView.dataSource
    property alias prevGridGroup: iconGridView.prevGrid
    property alias nextGridGroup: iconGridView.nextGrid
    property alias count: iconGridView.count
    property alias startIndex: iconGridView.iconIndexStart
    property alias endIndex: iconGridView.iconIndexEnd

    signal gridItemCountChanged
    signal gridCurrentItemChanged(variant newCurrentItem)

    // constants
    property int textToGridSpacing: constants.textToGridSpacing
    property int textHeight: constants.groupTextHeight

    spacing: textToGridSpacing
    width: parent.width
    height: childrenRect.height
    //visible: iconGridView.count != 0

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
    }

    Text {
        id: groupLabel
        width: parent.width
        height: text ? textHeight : 0
        maximumLineCount: 1
        anchors.left: parent.left
        anchors.leftMargin: 16

        font.family: "Bitstream Vera Sans"
        font.bold: true
        font.pixelSize: 18
        style: Text.Sunken
        color: "#eee"
        styleColor: "#000"
    }

    IconGridView {
        id: iconGridView

        width: parent.width
        height: Math.ceil(count / columns) * gridCellHeight

        interactive: false

        function updateSelection()
        {
            //parent.count = count // without this it is updated too late// TEST WHAT THE HELL NOW'S HAPPENING
            if (highlightItem)
            {
                highlightItem.animationDuration = 0
                highlightItem.opacity = 0
                highlightItem.animationDuration = 150
            }
            gridItemCountChanged()
        }

        function newItemData(iconPath, name, itemId, advParam)
        {
            if ((//startIndex === endIndex && endIndex === 0 ||
                     startIndex <= itemId && itemId <= endIndex &&
                     count <= endIndex - startIndex)) // Last condition eliminates duplicates via limiting item count. Not the best solution, fix someday
            {
                if (typeof advParam == 'boolean') // New item from RecentApps, advParam is pinned state of icon
                {
                    model.append({ imagePath: iconPath, caption: name, id: itemId, pinned: advParam })
                }
                else if (typeof advParam == 'string') // New item from search, advParam is group
                {
                    if (advParam == groupName)
                    {
                        //console.log("Added " + name + " : " + itemId + " in [" + startIndex + "-" + endIndex + "] of " + count)
                        model.append({ imagePath: iconPath, caption: name, id: itemId, pinned: undefined })
                    }
                }
                else
                    model.append({ imagePath: iconPath, caption: name, id: itemId, pinned: undefined })
            }

            //console.log("--- Added [" + startIndex + " to " + endIndex + "] with id: " + itemId)
        }

        function onItemClicked(newIndex)
        {
            if (newIndex != -1)
            {
                var realIndex = model.get(newIndex).id
                if (groupName == "Recent Applications")
                {
                    draggingItemIndex = realIndex
                    model.move(newIndex, 0, 1)

                    for (var i = 0; i < model.count; i++)
                        if (model.get(i).id < realIndex)
                            model.get(i).id++
                        else if (model.get(i).id == realIndex)
                            model.get(i).id = 0

                    draggingItemIndex = -1
                }
            }
            dataSource.itemClicked(newIndex == -1 ? newIndex : realIndex)
        }


        Component.onCompleted: {
            if ('group' in model)
                model.group = groupName
            updateSelection()
        }

        onCountChanged: {
            updateSelection()
        }
    }
}
