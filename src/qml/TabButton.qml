import QtQuick 1.0

Rectangle {
    id: button

    width: 200
    height: 40
    
    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"

    property alias label: buttonLabel.text
    
    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: "button label"
    }
    
    signal buttonClick()
    onButtonClick: {
        console.log(buttonLabel.text + " clicked")
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
