import QtQuick 1.1

Item {
    id: dropbox

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
            dropboxListBody.hide();
        }
    }
    
    Rectangle {
        id: dropboxButtonBody
        anchors.fill: parent

        color: "#222"
        border.color: "black"
        border.width: 1
        radius: 5

        z: 10
        
        Text {
            id: dropboxButtonText

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
            id: dropboxButtonMouseArea

            anchors.fill: parent

            onPressed: {
                dropboxListBody.hide();
                sessionProvider.lock();
            }
        }
        
        Rectangle {
            id: dropboxDrop

            height: parent.height
            width: parent.height

            anchors.right: parent.right
            
            color: "#222"
            border.color: "black"
            border.width: 1
            radius: 5

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    dropboxListBody.toggle();
                }
            }
        }
    }

    Rectangle {
        id: dropboxListBody

        width: dropboxButtonBody.width
        height: 0

        property int unfoldedHeight: dropboxButtonBody.height * 2
        
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
            id: dropboxListSelection

            width: parent.width
            height: dropboxButtonBody.height
            y: dropboxButtonBody.height

            Text {
                id: dropBoxListSelectionText

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
                    dropboxListBody.hide();
                    sessionProvider.shutdown();
                }
            }
            
        }
    }
}
