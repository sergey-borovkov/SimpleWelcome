import QtQuick 1.1

Item {
    id: grid

    width: parent.width
    height: rowsTotal * 140 + 16 + 32
    
    property int iconsInRow: 7
    property int iconsTotal: 0
    property int rowsTotal: 0
    
    property variant _lastGridRow
    property variant _GridRowComp

    Column {
        id: gridRowsContainer
        width: parent.width
        height: childrenRect.height
        spacing: 16
    }

    
    Component.onCompleted: {
        init();
        changeGroup();
    }

    function init()
    {
        _GridRowComp = Qt.createComponent("GridRow.qml");
        if(_GridRowComp.status == Component.Error) {
            console.log("Component loading error: " + _GridRowComp.errorString());
        }
    }

    function reload()
    {
        changeGroup();
    }
    
    function clear()
    {
        for(var i = 0; i < gridRowsContainer.children.length; i++)
        {
            gridRowsContainer.children[i].destroy();
        }

        iconsTotal = 0;
        rowsTotal = 0;
    }
    
    function changeGroup(groupName)
    {
        if(groupName === undefined)
        {
            clear();
            
            var groups = _getRootGroups();

            for(var i = 0; i < groups.length; i++)
            {
                var button = addButtonRaw({ "label": groups[i], "name": groups[i], "type": "", "iconUrl": "image://generalicon/stock/inode-directory.png" });

                button.triggered.connect(changeGroup);
                
                //var button = _AppButtonComp.createObject(gridContainer, { "buttonColor": "lightgrey", "label": groups[i], "entryPath": "", "entityName": "", "iconUrl": "image://generalicon/stock/inode-directory" });
            }
        }        
        else        
        {
            clear();
            
            var groupEntity = appProvider.getEntity(groupName);

            if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
            return;
            
            //var groupEntries = groupEntity.entries;
            var groupEntries = _deepFlaten(groupName);

            if(groupEntries.length <= 0)
            return;
            
            for(var i = 0; i < groupEntries.length; i++)
            {
                var entity = appProvider.getEntity(groupEntries[i]);

                addEntity(entity);
            }
            //console.log("New group name: " + groupName);
        }
    }
   
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
    
    function addButtonRaw(buttonOpts)
    {
        if(rowsTotal == 0)
          _addRow();

        if(_lastGridRow.size >= iconsInRow)
          _addRow();
        
        return _lastGridRow.addButtonRaw(buttonOpts);
    }
    
    function _addRow()
    {
        _lastGridRow = _GridRowComp.createObject(gridRowsContainer, {"iconsInRow": iconsInRow});

        rowsTotal += 1;
    }


    function _getRootGroups()
    {
        var rootGroups = [];
        
        var fullRootGroups = appProvider.getRootGroups();
        
        for(var i = 0; i < fullRootGroups.length; i++)
        {
            var groupName = fullRootGroups[i];
            var groupEntity = appProvider.getEntity(groupName);

            if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
            continue;

            rootGroups.push(groupName);
        }

        return rootGroups;
    }

    function _deepFlaten(groupName)
    {
        //console.log("----");
        //console.log("groupName:", groupName);
        
        var entities = [];

        var groupEntity = appProvider.getEntity(groupName);

        //console.log("Checking for goodness..");
        if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
        return entities;
        //console.log("Good");

        var groupEntries = groupEntity.entries;

        //console.log("Walking through entries");
        for(var i = 0; i < groupEntries.length; i++)
        {
            var entity = appProvider.getEntity(groupEntries[i]);

            //console.log("Entity:", groupEntries[i]);
            
            if(entity.isApp)
            {
                entities.push(groupEntries[i]);
            }
            else
            {
                var childrenEntities = _deepFlaten(groupEntries[i]);
                //console.log("childrenEntities:", childrenEntities);
                for(var j = 0; j < childrenEntities.length; j++)
                {
                    entities.push(childrenEntities[j]);
                }
                //entities.concat(childrenEntities);  // TOTALY BROKEN!!!
            }
        }

        //console.log("----");

        //console.log("Entities contains:", entities);
        
        return entities;
    }
}
