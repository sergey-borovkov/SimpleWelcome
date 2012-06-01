import QtQuick 1.1

Item {
    id: screenGridRow

    width: parent.width
    height: childrenRect.height

    property int cols: 7
    property int size: screenGridRowContainer.children.length

    Row {
        id: screenGridRowContainer
        width: childrenRect.width
        height: childrenRect.height
        spacing: (parent.width - 120 * cols) / (cols + 1)
        anchors.left: parent.left
        anchors.leftMargin: spacing
    }

    function addObject(component, opts)
    {
        return component.createObject(screenGridRowContainer, opts);
    }
}
