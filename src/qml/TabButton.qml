import QtQuick 1.1

Item {
    id: button
    width: constants.iconSize >= 64 ? 200 : 180
    height: 27

    signal buttonClick()

    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"
    property bool active: false
    property bool pressable: false
    property bool pressedAndHovered: buttonMouseArea.pressed && buttonMouseArea.containsMouse

    property alias label: buttonLabel.text

    Text {
        id: buttonLabel
        anchors {
            centerIn: parent
            horizontalCenterOffset: pressable && pressedAndHovered ? 1 : 0
            verticalCenterOffset: pressable && pressedAndHovered ? 1 : 0
        }

        color: active ? "black" : "white"

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent

        hoverEnabled: true

        onClicked: buttonClick()
    }
}
