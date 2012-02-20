import QtQuick 1.1

Rectangle {
    id: groupGrid

    width: parent.width
    height: childrenRect.height

    property int iconsInRow: 7

    property variant _GridGroupComp
    
    Column {
        id: groupGridContainer
        anchors.fill: parent
        height: 0
        spacing: 64
    }

    function preload()
    {
        _GridGroupComp = Qt.createComponent("GridGroup.qml");
        if(_GridGroupComp.status == Component.Ready) {
            load();
        } else if(_GridGroupComp.status == Component.Error) {
            console.log("Component loading error: " + _GridGroupComp.errorString());
        }
    }

    function load()
    {
        var rootGroups = _getRootGroups();

        for(var i = 0; i < rootGroups.length; i++)
        {
            fillGroup(rootGroups[i]);
        }
    }

    function fillGroup(groupName)
    {
        var groupEntity = appProvider.getEntity(groupName);

        if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
          return;
        
        //var groupEntries = groupEntity.entries;
        var groupEntries = _deepFlaten(groupName);

        if(groupEntries.length <= 0)
          return;
        
        var gridGroup = _GridGroupComp.createObject(groupGridContainer, {"iconsInRow": iconsInRow});

        gridGroup.label = groupEntity.name;
        
        for(var i = 0; i < groupEntries.length; i++)
        {
            var entity = appProvider.getEntity(groupEntries[i]);

            gridGroup.addEntity(entity);
        }

        height += gridGroup.height + 64;
        console.log("Group name:", groupName);
        console.log("Group height:", gridGroup.height);
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
