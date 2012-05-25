import QtQuick 1.1

Item {
    id: screenGrid

    width: parent.width // probably not needed
    height: childrenRect.height // probably not needed

    property int cols: 7
    property int rows: 4

    property int iconsTotal: 0
    property int rowsTotal: 0

    property variant _screenGridRowComp
    property variant _lastScreenGridRow

    Component.onCompleted: preload();

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(128,128,128,0.3)
    }

    Column {
        id: screenGridRowContainer
        anchors.fill: parent
        spacing: 64
    }

    function preload()
    {
        _screenGridRowComp = Qt.createComponent("ScreenGridRow.qml");
        if(_screenGridRowComp.status == Component.Ready)
        {
            init();
        }
        else if(_screenGridRowComp.status == Component.Error)
        {
            console.log("Component loading error: " + _screenGridRowComp.errorString());
        }
    }

    function init()
    {
    }

    function clear()
    {
        for(var i = 0; i < screenGridRowContainer.children.length; i++)
        {
            screenGridRowContainer.children[i].destroy();
        }

        iconsTotal = 0;
        rowsTotal = 0;
    }

    function addObject(component, opts)
    {
        if(rowsTotal == 0)
          _addRow();

        /*if(rowsTotal >= rows && _lastScreenGridRow.size >= cols)
          return;*/

        if(_lastScreenGridRow.size >= cols)
          _addRow();

        return _lastScreenGridRow.addObject(component, opts);
    }

    function _addRow()
    {
        _lastScreenGridRow = _screenGridRowComp.createObject(screenGridRowContainer, {"cols": cols});

        rowsTotal += 1;
        console.log("ROWS: " + rowsTotal)
    }

    function full()
    {
        if(rowsTotal >= rows && _lastScreenGridRow.size >= cols)
          return true;

        return false;
    }
}
