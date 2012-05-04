// ---

var GridGroupComp;
var GridGroup;

function prestartup()
{
    GridGroupComp = Qt.createComponent("GridGroup.qml");

    if(GridGroupComp.status == Component.Ready) {
        startup();
    } else if(GridGroupComp.status == Component.Error) {
        console.log("Component loading error: " + GridGroupComp.errorString());
    } else {
        GridGroupComp.statusChanged.connect(startup);
    }
    
}

function startup()
{
    GridGroup = GridGroupComp.createObject(rowContainer, {"iconsInRow": 7});

    var entityNames = appProvider.getEntityNames();

    for(var i = 0; i < 12; i++)
    {
        var entity = appProvider.getEntity(entityNames[i]);
        GridGroup.addEntity(entity);
        
    }
}
