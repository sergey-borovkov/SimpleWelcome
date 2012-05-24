import QtQuick 1.1

Item {
    id: applicationsTab
    width: parent.width

    clip: true
    anchors.topMargin: 16

    property variant _buttonComp

    Component.onCompleted: preload();

    function preload()
    {
        _buttonComp = Qt.createComponent("Button.qml");
        if(_buttonComp.status == Component.Ready)
        {
            init();
        }
        else if(_buttonComp.status == Component.Error)
        {
            console.log("Component loading error: " + _buttonComp.errorString());
        }
    }

    function init()
    {
        reload();
    }

    function reload()
    {
        setGroup();
    }

    function setGroup(groupName)
    {
        if(!groupName)
        {
            screenView.clear();

            var groups = _getRootGroups();

            for(var i = 0; i < groups.length; i++)
            {
                var groupEntries = _deepFlaten(groups[i]);

                if(groupEntries.length <= 0)
                  continue;

                var button = screenView.addObject(_buttonComp,
                {
                    "label": groups[i],
                    "iconUrl": "image://generalicon/stock/inode-directory.png"
                });

                function createGroupCallback(data)
                {
                    var name = data;

                    function callback()
                    {
                        applicationsTab.setGroup(name);
                    }

                    return callback;
                }

                button.buttonClick.connect(createGroupCallback(groups[i]));
            }
        }
        else
        {
            screenView.clear();

            var groupEntity = appProvider.getEntity(groupName);

            if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
              return;

            var groupEntries = _deepFlaten(groupName);

            if(groupEntries.length <= 0)
              return;

            for(var i = 0; i < groupEntries.length; i++)
            {
                var entity = appProvider.getEntity(groupEntries[i]);

                if(!entity.isApp)
                  continue;

                var button = screenView.addObject(_buttonComp,
                {
                    "label": entity.name,
                    "iconUrl": "image://generalicon/appicon/" + entity.iconName
                });

                function createEntityCallback(data)
                {
                    var entityName = data;

                    function callback()
                    {
                        appProvider.runEntity(entityName);
                    }

                    return callback;
                }

                button.buttonClick.connect(createEntityCallback(entity.name));
            }
        }
    }

    ScreenView
    {
        id: screenView

        cols: 7
        rows: 4
    }

    Rectangle {
        id: backButton

        width: 48
        height: 48

        anchors.left: parent.left
        anchors.top: parent.top

        anchors.leftMargin: 4
        anchors.topMargin: 4

        color: "#224"
        opacity: 0.8
        border.color: "#aaa"
        border.width: 1
        radius: 2

        z: 10

        MouseArea {
            id: backButtonMouseArea

            anchors.fill: parent

            onClicked: {
                applicationsTab.setGroup();
            }
        }
    }

    // -- Utilities

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
        var entities = [];

        var groupEntity = appProvider.getEntity(groupName);

        if(!groupEntity || groupEntity.entries.length <= 0 || groupEntity.isApp)
        return entities;

        var groupEntries = groupEntity.entries;

        for(var i = 0; i < groupEntries.length; i++)
        {
            var entity = appProvider.getEntity(groupEntries[i]);

            if(entity.isApp)
            {
                entities.push(groupEntries[i]);
            }
            else
            {
                var childrenEntities = _deepFlaten(groupEntries[i]);

                for(var j = 0; j < childrenEntities.length; j++)
                {
                    entities.push(childrenEntities[j]);
                }
            }
        }

        return entities;
    }

}
