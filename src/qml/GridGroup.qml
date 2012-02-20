import QtQuick 1.1

Rectangle {
    id: gridGroup

    width: parent.width
    //height: childrenRect.height
    //height: 140
    height: rowsTotal * 140 + 16 + 32

    property int iconsInRow: 7
    property int iconsTotal: 0
    property int rowsTotal: 0

    property alias label: groupLabel.text
    
    property variant _lastGridRow

    function addEntity(entity)
    {
        if(rowsTotal == 0)
          _addRow();

        if(_lastGridRow.size >= iconsInRow)
          _addRow();
        
        _lastGridRow.addEntity(entity);
    }

    function _addRow()
    {
        var GridRowComp = Qt.createComponent("GridRow.qml");

        _lastGridRow = GridRowComp.createObject(gridGroupContainer, {"iconsInRow": iconsInRow});

        rowsTotal += 1;
    }

    Column {
        id: gridGroupContainer
        anchors.fill: parent
        spacing: 16
        height: childrenRect.height

        Text {
            id: groupLabel
            width: parent.width
            height: 16
            maximumLineCount: 1
            clip: true
            horizontalAlignment: Text.AlignLeft
            anchors.left: parent.left
            anchors.leftMargin: 16

            text: "Group name 2"
        }
    }
}
