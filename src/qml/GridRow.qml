import QtQuick 1.1

Item {
    id: gridRow

    width: parent.width
    height: 140

    property int iconsInRow: 7
    property int size: _size()

    property variant _AppButtonComp

    Component.onCompleted: init();

    function init()
    {
        _AppButtonComp = Qt.createComponent("AppButton.qml");
        if(_AppButtonComp.status == Component.Error) {
            console.log("Component loading error: " + _AppButtonComp.errorString());
        }
    }    
    
    Row {
        id: gridRowContainer
        anchors.fill: parent
        spacing: (parent.width - 120 * iconsInRow) / (iconsInRow + 1)
        anchors.leftMargin: spacing
    }

    function addEntity(entity)
    {
        if(entity.isApp)
        {
            //console.log(entity.name);
            var button = _AppButtonComp.createObject(gridRowContainer, { "buttonColor": "lightgrey", "label": entity.name, "entryPath": entity.entryPath, "entityName": entity.name, "iconUrl": "image://appicon/" + entity.iconName });
        }        
    }

    function addQueryMatch(queryMatchName)
    {
        var button = _AppButtonComp.createObject(gridRowContainer, { "buttonColor": "lightgrey", "label": queryMatchName, "queryMatchName": queryMatchName, "iconUrl": "image://generalicon/search/" + queryMatchName });
    }

    function addButton(buttonOpts)
    {
        var button = _AppButtonComp.createObject(gridRowContainer, { "buttonColor": "lightgrey", "label": buttonOpts.name, "name": buttonOpts.name, "type": buttonOpts.type, "iconUrl": "image://generalicon/" + buttonOpts.type + "/" + buttonOpts.name });
    }

    function addButtonRaw(buttonOpts)
    {
        var button = _AppButtonComp.createObject(gridRowContainer, buttonOpts);

        return button;
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
