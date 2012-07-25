import QtQuick 1.1

Column {
    property alias groupName: groupLabel.text
    property alias gridDataSource: iconGridView.dataSource
    property alias prevGridGroup: iconGridView.prevGrid
    property alias nextGridGroup: iconGridView.nextGrid

    property alias gridView: iconGridView
    property alias count: iconGridView.count
    signal gridItemCountChanged
    signal gridCurrentItemChanged(variant newCurrentItem)


    spacing: 16
    //width: parent.width
    //height: childrenRect.height
    //visible: iconGridView.count != 0

    Component.onCompleted: {
        iconGridView.selectionChangedByKeyboard.connect(gridCurrentItemChanged)
    }

    Text {
        id: groupLabel
        width: parent.width
        height: 24
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
        height: Math.ceil(count / columns) * 200

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

        function newItemData(iconPath, name, itemId, group) {
            if (group === undefined || group == groupName)
                model.append( { imagePath: iconPath, caption: name, id: itemId})
        }


        Component.onCompleted: {
            if ('group' in model)
                model.group = groupName
            updateSelection()
        }

        Behavior on height {
            NumberAnimation { duration: 150 }
        }

        onCountChanged: {
            updateSelection()
        }
    }
}
