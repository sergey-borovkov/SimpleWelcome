import QtQuick 1.1

Item {
    id: cell

    // constants
    property int cellWidth: constants.cellWidth

    width: cellWidth
    height: wrapper.height//140

    Item {
        id: wrapper
        parent: gridMouseArea
        x: cell.x
        y: cell.y
        width: cellWidth
        height: childrenRect.height

        Image {
            id: cellIcon
            source: imagePath
            width: constants.iconSize
            height: constants.iconSize
            anchors.top: parent.top
            anchors.topMargin: constants.iconSize / 5 //20
            anchors.horizontalCenter: parent.horizontalCenter

            smooth: true
        }

        Text {
            id: cellText
            text: caption

            width: parent.width - 10
            anchors.top: cellIcon.bottom
            anchors.topMargin: constants.iconSize / 5 //24
            anchors.horizontalCenter: parent.horizontalCenter

            style: Text.Sunken
            styleColor: "#000"
            color: "#eee"
            font.bold: true
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize

            maximumLineCount: 3
            elide: Text.ElideRight
            clip: true
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }

        /*states: [
            State {
            name: "active";
            when: cell.activeFocus
            PropertyChanges { target: cellIcon; scale: 1.2 }
        },
        */

        Behavior on x { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }
        Behavior on y { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }

        states: [
            State {
                name: "cellActive";
                when: cell.activeFocus && gridMouseArea.draggingItemIndex != id
                PropertyChanges { target: cellIcon; scale: 1.2 }
            },

            State {
                name: "gridInDrag"
                when: gridMouseArea.draggingItemIndex != -1 && gridMouseArea.draggingItemIndex != id
            },

            State {
                name: "cellInDrag"
                when: gridMouseArea.draggingItemIndex == id
                PropertyChanges { target: wrapper; parent: 0 }
                PropertyChanges {
                    target: wrapper;
                    x: gridMouseArea.mouseX - wrapper.width/2;
                    y: gridMouseArea.mouseY - wrapper.height/2;
                    z: 10
                }
                PropertyChanges { target: cellIcon; scale: 1.2 }
            }
        ]

        transitions: Transition {
            NumberAnimation { properties: "scale"; duration: 200 }
            NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
        }

        Item { // Necessary for correct highlight height
            height: cellIcon.anchors.topMargin + cellIcon.height + cellText.anchors.topMargin + cellText.height + cellIcon.anchors.topMargin/2
        }
    }
}
