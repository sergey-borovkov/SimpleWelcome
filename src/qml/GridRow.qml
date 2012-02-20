import QtQuick 1.1

Rectangle {
    id: gridRow

    width: parent.width
    height: 140

    property int iconsInRow: 7
    property int size: _size()
    
    Row {
        id: gridRowContainer
        anchors.fill: parent
        spacing: (parent.width - 120 * iconsInRow) / (iconsInRow + 1)
        anchors.leftMargin: spacing
    }

    function addEntity(entity)
    {
        var AppButtonComp = Qt.createComponent("AppButton.qml"); // Should be called only once somehow
        
        if(entity.isApp)
        {
            //console.log(entity.name);
            var button = AppButtonComp.createObject(gridRowContainer, { "buttonColor": "lightgrey", "label": entity.name, "entryPath": entity.entryPath, "entityName": entity.name, "iconUrl": "image://appicon/" + entity.iconName });
        }        
    }

    function _size()
    {
        return gridRowContainer.children.length;
    }
}
