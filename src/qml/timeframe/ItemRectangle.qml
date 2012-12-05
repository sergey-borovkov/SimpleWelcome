import QtQuick 1.0

Item {
    property alias color : rectangle.color
    property alias radius : rectangle.radius

    Rectangle {
        id: rectangle;
        anchors.fill: parent;
        radius: 3;
        color: "#172634"
        border {
            color: "#687584"
            width: 1
        }

        BorderImage {
            z: -1
            anchors {
                bottomMargin: -8
                fill: parent
            }
            width: rectangle.width
            height: rectangle.height
            border {
                left: 10
                top: 10
                right: 10
                bottom: 10
            }
            source: "images/shadow.png"; smooth: true
        }
    }
    states: [
        State {
            name: "notEnabled"
            PropertyChanges {
                target: rectangle
                color: "#2A3947"
                border.color: "#4B5E6E"
            }
        }
    ]
}
