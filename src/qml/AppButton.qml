import QtQuick 1.1

Rectangle {
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

    Image {
        id: buttonIcon
        source: "image://appicon/yellow"
        width: 64
        height: 64
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
    }
    
    Text {
        id: buttonLabel
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 16 + 64 + 16

        maximumLineCount: 3
        elide: Text.ElideRight
        clip: true
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap

        text: "button label"
    }
    
    signal buttonClick()
    onButtonClick: {
        console.log(buttonLabel.text + " - " + entryPath + " clicked")
        var tmpText = iconUrl
        iconUrl = ""
        iconUrl = tmpText
        appProvider.runEntity(entityName);
    }

    MouseArea {
        id: buttonMouseArea

        anchors.fill: parent
        
        onClicked: buttonClick()
        hoverEnabled: true
        onEntered: parent.border.color = onHoverColor
        onExited:  parent.border.color = borderColor
    }

    color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}
