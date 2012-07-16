import QtQuick 1.1

Item {
    id: cell

    width: 120
    height: childrenRect.height//140

    signal activated(bool isActive, variant element)

    Image {
        id: cellIcon
        source: imagePath
        width: 64
        height: 64
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter

        smooth: true

        states: [
            State {
                name: "NORMAL"
                PropertyChanges { target: cellIcon; width: 64 }
                PropertyChanges { target: cellIcon; height: 64 }
                PropertyChanges { target: cellIcon; anchors.topMargin: 8 }
            },
            State {
                name: "HOVER"
                PropertyChanges { target: cellIcon; width: 72 }
                PropertyChanges { target: cellIcon; height: 72 }
                PropertyChanges { target: cellIcon; anchors.topMargin: 4 }
            }
        ]

        transitions: Transition {
            NumberAnimation { properties: "width,height,anchors.topMargin,opacity"; easing.type: Easing.OutQuad }
        }
    }

    Text {
        id: cellLabel
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 16 + 64 + 16
        style: Text.Sunken
        styleColor: "#000"
        color: "#eee"
        font.bold: true
        font.family: "Bitstream Vera Sans"

        maximumLineCount: 3
        elide: Text.ElideRight
        clip: true
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap

        text: caption//parent.label
    }

    states: State {
        name: "active"; when: cell.activeFocus
        PropertyChanges { target: cellIcon; scale: 1.2 }
    }

    transitions: Transition {
        NumberAnimation { properties: "scale"; duration: 200 }
    }

    MouseArea {
        id: cellMouseArea

        anchors.fill: parent

        onClicked: buttonClick(clickCallbackData)
        onEntered: hover(true)
        onExited: hover(false)
        hoverEnabled: true
    }

    function hover(pop)
    {
        if(pop)
        {
            cellIcon.state = "HOVER"
        }
        else
        {
            cellIcon.state = "NORMAL"
        }
        activated(pop, cell)
    }
}
