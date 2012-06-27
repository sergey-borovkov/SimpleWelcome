import QtQuick 1.0

Item {
    id: button

    width: 200
    height: 36

    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"

    property alias label: buttonLabel.text

    BorderImage {
        border.left: 6
        border.right: 6
        border.top: 6
        border.bottom: 6
        anchors.fill: parent
        source: "image://generalicon/asset/button_small_border_bg.png"
    }

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: "button label"
        style: Text.Sunken
        styleColor: "#000"
        color: "#eee"
        font.bold: true
        font.family: "Bitstream Vera Sans"
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
