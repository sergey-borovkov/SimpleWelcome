import QtQuick 1.1

Item {
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
    property variant _GridRowComp

    function addEntity(entity)
    {
        if(rowsTotal == 0)
          _addRow();

        if(_lastGridRow.size >= iconsInRow)
          _addRow();
        
        _lastGridRow.addEntity(entity);
    }

    function addQueryMatch(queryMatchName)
    {
        if(rowsTotal == 0)
          _addRow();

        if(_lastGridRow.size >= iconsInRow)
          _addRow();
        
        _lastGridRow.addQueryMatch(queryMatchName);
    }

    function addButton(buttonOpts)
    {
        if(rowsTotal == 0)
          _addRow();

        if(_lastGridRow.size >= iconsInRow)
          _addRow();
        
        _lastGridRow.addButton(buttonOpts);
    }
    
    function _addRow()
    {
        _GridRowComp = Qt.createComponent("GridRow.qml");

        if(_GridRowComp.status == Component.Error) {
            console.log("Component loading error: " + _GridRowComp.errorString());
        }

        _lastGridRow = _GridRowComp.createObject(gridGroupContainer, {"iconsInRow": iconsInRow});

        rowsTotal += 1;
    }

    Column {
        id: gridGroupContainer
        spacing: 16
        width: parent.width
        height: childrenRect.height

        Text {
            id: groupLabel
            width: parent.width
            height: 24
            maximumLineCount: 1
            clip: true
            horizontalAlignment: Text.AlignLeft
            anchors.left: parent.left
            anchors.leftMargin: 16

            font.family: "Bitstream Vera Sans"
            font.bold: true
            font.pixelSize: 18
            style: Text.Sunken
            color: "#eee"
            styleColor: "#000"


            text: "Group name 2"
        }
    }
}
