import QtQuick 1.1

Item {
    id: button

    width: 120
    height: childrenRect.height//140

    signal activated(bool isActive, variant element)

    Image {
        id: buttonIcon
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
                PropertyChanges { target: buttonIcon; width: 64 }
                PropertyChanges { target: buttonIcon; height: 64 }
                PropertyChanges { target: buttonIcon; anchors.topMargin: 8 }
            },
            State {
                name: "HOVER"
                PropertyChanges { target: buttonIcon; width: 72 }
                PropertyChanges { target: buttonIcon; height: 72 }
                PropertyChanges { target: buttonIcon; anchors.topMargin: 4 }
            }
        ]

        transitions: Transition {
            NumberAnimation { properties: "width,height,anchors.topMargin,opacity"; easing.type: Easing.OutQuad }
        }
    }

    Text {
        id: buttonLabel
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

    signal buttonClick()
    onButtonClick: function(data) {
        console.log("BLABLA: " + data);
    }

    MouseArea {
        id: buttonMouseArea

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
            buttonIcon.state = "HOVER"
        }
        else
        {
            buttonIcon.state = "NORMAL"
        }
        activated(pop, button)
    }
}
