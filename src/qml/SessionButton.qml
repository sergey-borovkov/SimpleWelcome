import QtQuick 1.1

Item {
    id: sessionButton

    property alias imgNormal: normalButton.source
    property alias imgHover: hoverButton.source
    property alias imgPressed: pressedButton.source

    function onButtonClicked() { }

    Image {
        id: normalButton
        anchors.centerIn: parent
        sourceSize.width: parent.width
        sourceSize.height: parent.height
    }

    Image {
        id: hoverButton
        anchors.centerIn: parent
        sourceSize.width: parent.width
        sourceSize.height: parent.height

        opacity: 0

        Behavior on opacity {
            NumberAnimation { duration: 100; }
        }
    }

    Image {
        id: pressedButton
        anchors.centerIn: parent
        sourceSize.width: parent.width
        sourceSize.height: parent.height

        opacity: 0
    }

    MouseArea {
        id: sessionButtonMouseArea
        anchors.fill: parent

        hoverEnabled: true

        onClicked: onButtonClicked()
    }

    states: [
        State {
            name: "normal";
            when: !sessionButtonMouseArea.containsMouse
            PropertyChanges { target: hoverButton; opacity: 0; }
        },

        State {
            name: "hover";
            when: sessionButtonMouseArea.containsMouse && !sessionButtonMouseArea.pressed
            PropertyChanges { target: hoverButton; opacity: 1 }
        },

        State {
            name: "pressed"
            when: sessionButtonMouseArea.pressed
            PropertyChanges { target: pressedButton; opacity: 1 }
        }
    ]
}
