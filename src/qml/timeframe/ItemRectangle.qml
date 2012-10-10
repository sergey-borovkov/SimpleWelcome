import QtQuick 1.0

Item {
//    height: 200
    property alias color : rectangle.color
    property alias radius : rectangle.radius
    //anchors { leftMargin: -6; topMargin: -6; rightMargin: -8; bottomMargin: -8 }

    Rectangle {
        id: rectangle;
        anchors.fill: parent;
        radius: 10;
        color: "#172634"
        border.color: "#687584"
        border.width: 1
        BorderImage {
            z: -1
            anchors.fill: parent
            width: rectangle.width
            height: rectangle.height
            anchors { bottomMargin: -8 }
            border { left: 10; top: 10; right: 10; bottom: 10 }
            source: "images/shadow.png"; smooth: true
        }
    }
}
