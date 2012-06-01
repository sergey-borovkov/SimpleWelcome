import QtQuick 1.1

Item {
    id: screenGrid

    width: parent.width
    height: childrenRect.height

    property int cols: 7
    property int rows: 4

    property int iconsTotal: 0
    property int rowsTotal: 0

    property variant _screenGridRowComp
    property variant _lastScreenGridRow

    Component.onCompleted: preload();

    Column {
        id: screenGridRowContainer
        width: parent.width
        height: childrenRect.height
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
    }

    function full()
    {
        if(rowsTotal >= rows && _lastScreenGridRow.size >= cols)
          return true;

        return false;
    }
}
