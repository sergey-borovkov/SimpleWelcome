import QtQuick 1.1

Item {
    id: button

    width: 120
    height: 140
    
    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"

    property string entityName: ""
    property string entryPath: ""
    
    property alias label: buttonLabel.text
    property alias iconUrl: buttonIcon.source

    property string queryMatchName: ""
    property string type: "app"
    property string name: ""

    signal triggered(string name)
    
    /*
    Image {
        id: buttonIconBackground
        source: "image://generalicon/asset/button_bg.png"
        width: 78
        height: 78
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }
    */

    Image {
        id: buttonIconBackground
        source: "image://generalicon/asset/button.png"
        width: 78
        height: 78
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }
    
    Image {
        id: buttonIcon
        source: "image://appicon/yellow"
        width: 64
        height: 64
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
    }

    /*
    Image {
        id: buttonIconBackgroundGlare
        source: "image://generalicon/asset/button_glare.png"
        width: 78
        height: 78
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }
    */
    
    Text {
        id: buttonLabel
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 16 + 64 + 16
        style: Text.Sunken
        styleColor: "#000"
        color: "#eee"
        font.bold: true
        font.family: "Bitstream Vera Sans"

        maximumLineCount: 3
        elide: Text.ElideRight
        clip: true
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap

        text: "button label"
    }
    
    signal buttonClick()
    onButtonClick: {
        //console.log(buttonLabel.text + " - " + buttonLabel.text + " clicked")
        /*
        var tmpText = iconUrl
        iconUrl = ""
        iconUrl = tmpText
        */
        if(type == "recentApp")
        {
            recentAppsProvider.runRecentApp(name);
        }
        else if(type == "place")
        {
            placesProvider.runPlace(name);
        }
        else if(type == "document")
        {
            documentsProvider.runDoc(name);
        }
        else if(queryMatchName.length == 0) // If app
        {
            appProvider.runEntity(entityName);
        }
        else // If queryMatch
        {
            searchRunner.runMatch(queryMatchName);
        }

        page.reloadTabs();

        button.triggered(button.name);
        
        //Qt.quit();
    }

    MouseArea {
        id: buttonMouseArea

        anchors.fill: parent
        
        onClicked: buttonClick()
        hoverEnabled: true
        //onEntered: parent.border.color = onHoverColor
        //onExited:  parent.border.color = borderColor
    }

    //color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}
