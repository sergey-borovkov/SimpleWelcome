import QtQuick 1.1

Item {
    id: groupGrid

    width: parent.width
    height: childrenRect.height

    property int iconsInRow: 7
    property string gridType: "apps"

    property variant _GridGroupComp
    property variant _buttonComp
    
    Column {
        id: groupGridContainer
        anchors.fill: parent
        height: 0
        spacing: 64
    }

    function preload()
    {
        _buttonComp = Qt.createComponent("Button.qml");
        if(_buttonComp.status == Component.Error)
        {
            console.log("Component loading error: " + _buttonComp.errorString());
        }

        _GridGroupComp = Qt.createComponent("GridGroup.qml");
        if(_GridGroupComp.status == Component.Ready) {
            load();
        } else if(_GridGroupComp.status == Component.Error) {
            console.log("Component loading error: " + _GridGroupComp.errorString());
        }
    }

    function load()
    {
        if(gridType == "apps")
        {
            reloadApps();
        }
        if(gridType == "welcome")
        {
            reloadWelcome();
        }
    }

    function clear()
    {
        for(var i = 0; i < groupGridContainer.children.length; i++)
        {
            groupGridContainer.children[i].destroy();
        }
    }
    
    function addGroup(name)
    {
        var gridGroup = _GridGroupComp.createObject(groupGridContainer, {"iconsInRow": iconsInRow});

        gridGroup.label = name;

        return gridGroup;
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
        //console.log("Group name:", groupName);
        //console.log("Group height:", gridGroup.height);
    }

    function reloadApps()
    {
        //console.log("reloadApps called");
        
        // Clearing
        clear();

        // Loading
        var rootGroups = _getRootGroups();

        for(var i = 0; i < rootGroups.length; i++)
        {
            fillGroup(rootGroups[i]);
        }
    }
    
    function reloadWelcome()
    {
        //console.log("reloadWelcome called");
        
        // Clearing
        clear();
        
        // RecentApps

        var gridGroup = _GridGroupComp.createObject(groupGridContainer, {"iconsInRow": iconsInRow});
        gridGroup.label = qsTr("Recent Applications");

        var recentAppNames = recentAppsProvider.getRecentAppsList();
        //console.log(recentAppNames);

        for(var i = 0; i < recentAppNames.length; i++)
        {
            //gridGroup.addButton({"type": "recentApp", "name": recentAppNames[i]});
            var appName = recentAppNames[i];
            
            var button = gridGroup.addObject(_buttonComp,
            {
                "label": appName,
                "iconUrl": "image://generalicon/recentApp/" + appName
            });

            function createRecentAppCallback(data)
            {
                var name = data;

                function callback()
                {
                    recentAppsProvider.runRecentApp(name);
                }

                return callback;
            }

            button.buttonClick.connect(createRecentAppCallback(appName));
        }

        // Places
        gridGroup = _GridGroupComp.createObject(groupGridContainer, {"iconsInRow": iconsInRow});
        gridGroup.label = qsTr("Places");

        var placesNames = placesProvider.getPlacesList();

        for(var i = 0; i < placesNames.length; i++)
        {
            //gridGroup.addButton({"type": "place", "name": placesNames[i]});
            var placeName = placesNames[i];

            var button = gridGroup.addObject(_buttonComp,
            {
                "label": placeName,
                "iconUrl": "image://generalicon/place/" + placeName
            });

            function createPlaceCallback(data)
            {
                var name = data;

                function callback()
                {
                    placesProvider.runPlace(name);
                }

                return callback;
            }

            button.buttonClick.connect(createPlaceCallback(placeName));
        }
        
        // Documents
        gridGroup = _GridGroupComp.createObject(groupGridContainer, {"iconsInRow": iconsInRow});
        gridGroup.label = qsTr("Documents");

        var documentNames = documentsProvider.getDocsList();

        for(var i = 0; i < documentNames.length; i++)
        {
            //gridGroup.addButton({"type": "document", "name": documentNames[i]});
            var documentName = documentNames[i];

            var button = gridGroup.addObject(_buttonComp,
            {
                "label": documentName,
                "iconUrl": "image://generalicon/document/" + documentName
            });

            function createDocumentCallback(data)
            {
                var name = data;

                function callback()
                {
                    documentsProvider.runDoc(name);
                }

                return callback;
            }

            button.buttonClick.connect(createDocumentCallback(documentName));            
        }
        
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
