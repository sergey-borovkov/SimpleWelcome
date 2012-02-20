import QtQuick 1.0

Rectangle {
    width: parent.width
    height: 800
    clip: true
    //anchors.top: parent.top
    anchors.topMargin: 16

    function startup() {
        var entityNames = appProvider.getEntityNames();

        var AppButtonComp = Qt.createComponent("AppButton.qml");
        
        for(var i = 0; i < entityNames.length; i++)
        {
            var entity = appProvider.getEntity(entityNames[i]);
            if(entity && entity.isApp)
            {
                //console.log(entity.name);
                var button = AppButtonComp.createObject(itemContainer, { "buttonColor": "lightgrey", "label": entity.name, "entryPath": entity.entryPath, "entityName": entity.name, "iconUrl": "image://appicon/" + entity.iconName });
            }
        }
    }

    Component.onCompleted: startup();
    
    Flickable {
        id: view
        anchors.fill:parent
        contentWidth: parent.width
        contentHeight: 2000
        
        Column {
            id: rowContainer
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 16
            spacing: 16
            width: parent.width

            Grid {
                id: itemContainer
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (parent.width - 120*7) / 8
                columns: 7

                //AppButton { id: firstButton; buttonColor: "lightgrey"; label: "An App" }
            }
            
            /*
            Row {
                id: itemContainer
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (parent.width - 120*7) / 8

                AppButton { id: firstButton; buttonColor: "lightgrey"; label: "An App" }
                AppButton { id: secondButton; buttonColor: "grey"; label: "An App 2" }
                AppButton { id: thirdButton; buttonColor: "darkgrey"; label: "An App 3" }
                AppButton { id: forthButton; buttonColor: "lightgrey"; label: "An App 4" }
                AppButton { id: fifthButton; buttonColor: "grey"; label: "An App 5" }
                AppButton { id: sixthButton; buttonColor: "darkgrey"; label: "An App 6" }
                AppButton { id: seventhButton; buttonColor: "lightgrey"; label: "An App 7" }
            }
            
            Row {
                id: itemContainer2
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (parent.width - 120*7) / 8

                AppButton { id: firstButton1; buttonColor: "lightgrey"; label: "An App" }
                AppButton { id: secondButton1; buttonColor: "grey"; label: "An App 2" }
                AppButton { id: thirdButton1; buttonColor: "darkgrey"; label: "An App 3" }
                AppButton { id: forthButton1; buttonColor: "lightgrey"; label: "An App 4" }
                AppButton { id: fifthButton1; buttonColor: "grey"; label: "An App 5" }
                AppButton { id: sixthButton1; buttonColor: "darkgrey"; label: "An App 6" }
                AppButton { id: seventhButton1; buttonColor: "lightgrey"; label: "An App 7" }
            }
            
            Row {
                id: itemContainer3
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (parent.width - 120*7) / 8

                AppButton { id: firstButton2; buttonColor: "lightgrey"; label: "An App" }
                AppButton { id: secondButton2; buttonColor: "grey"; label: "An App 2" }
                AppButton { id: thirdButton2; buttonColor: "darkgrey"; label: "An App 3" }
                AppButton { id: forthButton2; buttonColor: "lightgrey"; label: "An App 4" }
                AppButton { id: fifthButton2; buttonColor: "grey"; label: "An App 5" }
                AppButton { id: sixthButton2; buttonColor: "darkgrey"; label: "An App 6" }
                AppButton { id: seventhButton2; buttonColor: "lightgrey"; label: "An App 7" }
            }
            */
        }

        states: State {
            name: "ShowBars"
            when: view.movingVertically || view.movingHorizontally
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
            PropertyChanges { target: horizontalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }

    ScrollBar {
        id: verticalScrollBar
        width: 12;
        height: view.height - 12

        anchors.right: view.right
        opacity: 0
        orientation: Qt.Vertical
        position: view.visibleArea.yPosition
        pageSize: view.visibleArea.heightRatio
    }

    ScrollBar {
        id: horizontalScrollBar
        width: view.width - 12;
        height: 12
        
        anchors.bottom: view.bottom
        opacity: 0
        orientation: Qt.Horizontal
        position: view.visibleArea.xPosition
        pageSize: view.visibleArea.widthRatio
    }

}
