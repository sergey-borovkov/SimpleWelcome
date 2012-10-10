import QtQuick 1.0

Item {
    id: button

    width: constants.iconSize >= 64 ? 200 : 180
    height: 27

    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"
    property bool active: false

    property alias label: buttonLabel.text

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: "button label"
        color: active ? "black" : "white"

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    signal buttonClick()
    signal buttonPress()
    signal buttonRelease()

    onButtonClick: {
        //console.log(buttonLabel.text + " clicked")
    }

    MouseArea {
        id: buttonMouseArea

        anchors.fill: parent

        onClicked: buttonClick()
        onPressed: buttonPress()
        onReleased: buttonRelease()
        hoverEnabled: true
        //onEntered: parent.border.color = onHoverColor
        //onExited:  parent.border.color = borderColor
    }

    //color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}
