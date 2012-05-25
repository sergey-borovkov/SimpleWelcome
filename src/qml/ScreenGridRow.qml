import QtQuick 1.1

Item {
    id: screenGridRow

    width: parent.width
    height: 140

    property int cols: 7
    property int size: _size()

    Component.onCompleted: init();

    function init()
    {
    }

    Row {
        id: screenGridRowContainer
        anchors.fill: parent
        spacing: (parent.width - 120 * cols) / (cols + 1)
        anchors.leftMargin: spacing
    }

    function addObject(component, opts)
    {
        var object = component.createObject(screenGridRowContainer, opts);

        return object;
    }

    function _size()
    {
        return screenGridRowContainer.children.length;
    }
}
