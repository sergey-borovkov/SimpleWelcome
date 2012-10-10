import QtQuick 1.0

Item {
    id: button

    width: constants.iconSize >= 64 ? 200 : 180
    height: 27

    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"
    property bool active: false
    property bool pressable: false
    property bool pressedAndHovered: buttonMouseArea.pressed && buttonMouseArea.containsMouse

    property alias label: buttonLabel.text

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: pressable && pressedAndHovered ? 1 : 0
        anchors.verticalCenterOffset: pressable && pressedAndHovered ? 1 : 0
        text: "button label"
        color: active ? "black" : "white"

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    signal buttonClick()

    onButtonClick: {
        //console.log(buttonLabel.text + " clicked")
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
