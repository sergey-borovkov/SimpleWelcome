import QtQuick 1.1

Item {
    width: 100
    height: 38

    MouseArea {
        id: componentMouseArea
        anchors.left: parent.left
        anchors.top: parent.top

        width: parent.width
        height: parent.height * 2

        hoverEnabled: true

        onExited: {
            comboBoxListBody.hide();
        }
    }

    Rectangle {
        id: comboBoxButtonBody
        anchors.fill: parent

        color: "#222"
        border.color: "black"
        border.width: 1
        radius: 5

        z: 10

        Text {
            id: comboBoxButtonText

            x: 16
            anchors.verticalCenter: parent.verticalCenter

            maximumLineCount: 1
            elide: Text.ElideRight
            clip: true
            wrapMode: Text.Wrap
            font.pointSize: 14
            font.family: "Bitstream Vera Sans"
            style: Text.Raised
            styleColor: "#000"
            color: "#eee"

            text: "Lock screen"
        }

        MouseArea {
            id: comboBoxButtonMouseArea

            anchors.fill: parent

            onPressed: {
                comboBoxListBody.hide();
                sessionProvider.lock();
            }
        }

        Rectangle {
            id: comboBoxDrop

            height: parent.height // probably not needed
            width: parent.height // probably not needed
            anchors.fill: parent

            anchors.right: parent.right

            color: "#222"
            border.color: "black"
            border.width: 1
            radius: 5

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    comboBoxListBody.toggle();
                }
            }
        }
    }

    Rectangle {
        id: comboBoxListBody

        width: comboBoxButtonBody.width
        height: 0

        property int unfoldedHeight: comboBoxButtonBody.height * 2

        anchors.top: parent.top
        anchors.left: parent.left

        color: "#222"
        border.color: "black"
        border.width: 1
        radius: 5

        clip: true

        function toggle()
        {
            if(height == 0)
              height = unfoldedHeight;
            else
              height = 0;
        }

        function hide()
        {
            height = 0;
        }

        function show()
        {
            height = unfoldedHeight;
        }

        Item {
            id: comboBoxListSelection

            width: parent.width
            height: comboBoxButtonBody.height
            y: comboBoxButtonBody.height

            Text {
                id: comboBoxListSelectionText

                x: 16
                anchors.verticalCenter: parent.verticalCenter

                maximumLineCount: 1
                elide: Text.ElideRight
                clip: true
                wrapMode: Text.Wrap
                font.pointSize: 14
                font.family: "Bitstream Vera Sans"
                style: Text.Raised
                styleColor: "#000"
                color: "#eee"

                text: "Logout"
            }

            MouseArea {
                id: shutdownMouseArea

                anchors.fill: parent

                onPressed: {
                    comboBoxListBody.hide();
                    sessionProvider.shutdown();
                }
            }

        }
    }
}
