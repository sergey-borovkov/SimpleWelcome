import QtQuick 1.1

Item {
    id: gridRow

    width: parent.width
    height: childrenRect.height

    property int iconsInRow: 7
    property int size: _size()

    Component.onCompleted: init();

    function init()
    {
    }

    Row {
        id: gridRowContainer
        width: childrenRect.width
        height: childrenRect.height
        spacing: (parent.width - 120 * iconsInRow) / (iconsInRow + 1)
        anchors.left: parent.left
        anchors.leftMargin: spacing
    }

    function addObject(component, opts)
    {
        var object = component.createObject(gridRowContainer, opts);
        return object;
    }

    function _size()
    {
        return gridRowContainer.children.length;
    }
}
