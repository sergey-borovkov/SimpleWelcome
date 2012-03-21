import QtQuick 1.1

Item {
    id: searchTab
    width: parent.width
    clip: true
    anchors.topMargin: 16

    //signal newMatchesFound()
    
    Component.onCompleted: load();

    function load()
    {
        groupGrid.preload();
        searchRunner.newSearchMatchesFound.connect(newMatchesFound);
    }

    function newMatchesFound()
    {
        tabListView.currentIndex = 0
        //console.log("NewMatchesFound called");

        var matchesList = searchRunner.getMatchNames();
        var groupsList = searchRunner.getGroupNames();
        var groups = {};

        groupGrid.clear();
        
        for(var i = 0; i < groupsList.length; i++)
        {
            var groupName = groupsList[i];
            groups[groupName] = groupGrid.addGroup(groupName);
        }
        
        for(var i = 0; i < matchesList.length; i++)
        {
            var matchName = matchesList[i];
            var groupName = searchRunner.getMatchGroupName(matchName);

            groups[groupName].addQueryMatch(matchName);
            //console.log("Group:", searchRunner.getMatchGroupName(matchesList[i]), "Match:", matchesList[i]);
            
        }
    }

    function hideSearchTab()
    {
        tabListView.currentIndex = 1
    }
    
    Flickable {
        id: view
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: groupGrid.height
        
        Column {
            id: rowContainer
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 16
            spacing: 16
            width: parent.width

            GroupGrid {
                id: groupGrid
                gridType: "search"
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
        
}
