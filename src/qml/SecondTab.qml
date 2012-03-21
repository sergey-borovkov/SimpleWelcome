import QtQuick 1.0

//import "SecondTab.js" as Logic

Item {
    id: secondTab
    width: parent.width
    //height: 800
    clip: true
    //anchors.top: parent.top
    anchors.topMargin: 16

    ////Component.onCompleted: Logic.prestartup();
    //Component.onCompleted: groupGrid.preload();

    function reload()
    {
        appGrid.reload();
        //groupGrid.reloadApps();
    }
    
    Flickable {
        id: view
        anchors.fill: parent
        contentWidth: parent.width
        //contentHeight: groupGrid.height
        contentHeight: appGrid.height
        
        Column {
            id: rowContainer
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 16
            spacing: 16
            width: parent.width

            /*
            GroupGrid {
                id: groupGrid
            }
            */

            AppGrid {
                id: appGrid
                
            }
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
    
    /*
    Row {
        id: itemContainer
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: (parent.width - 120*7) / 8

        AppButton { id: firstButton; buttonColor: "lightgrey"; label: "An 2 App" }
        AppButton { id: secondButton; buttonColor: "grey"; label: "An 2 App 2" }
        AppButton { id: thirdButton; buttonColor: "darkgrey"; label: "An 2 App 3" }
        AppButton { id: forthButton; buttonColor: "lightgrey"; label: "An 2 App 4" }
        AppButton { id: fifthButton; buttonColor: "grey"; label: "An 2 App 5" }
        AppButton { id: sixthButton; buttonColor: "darkgrey"; label: "An 2 App 6" }
        AppButton { id: seventhButton; buttonColor: "lightgrey"; label: "An 2 App 7" }
    }
    
    Row {
        id: itemContainer2
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: (parent.width - 120*7) / 8

        AppButton { id: firstButton1; buttonColor: "lightgrey"; label: "An 2 App" }
        AppButton { id: secondButton1; buttonColor: "grey"; label: "An 2 App 2" }
        AppButton { id: thirdButton1; buttonColor: "darkgrey"; label: "An 2 App 3" }
        AppButton { id: forthButton1; buttonColor: "lightgrey"; label: "An 2 App 4" }
        AppButton { id: fifthButton1; buttonColor: "grey"; label: "An 2 App 5" }
        AppButton { id: sixthButton1; buttonColor: "darkgrey"; label: "An 2 App 6" }
        AppButton { id: seventhButton1; buttonColor: "lightgrey"; label: "An 2 App 7" }
    }
    */
    
}
