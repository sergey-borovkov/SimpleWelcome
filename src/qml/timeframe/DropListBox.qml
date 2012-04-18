import QtQuick 1.1

Item {
    id: dropListBox

    property string name;
    property string selectedText: dropboxListSelection.model.get( 0 ).itemText
    property alias model: dropboxListSelection.model
    property alias selectedIndex: dropboxListSelection.currentIndex

    signal comboClicked
    signal selectedIndexChanged

    width: dropboxButtonText.paintedWidth + 32 + dropboxDrop.width
    height: 30
    focus: true

    Button {
        id: dropboxButtonBody
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        z: 100

        ButtonText {
            id: dropboxButtonText
            text: name
            anchors.left: parent.left
        }

        MouseArea {
            id: dropboxButtonMouseArea
            anchors.fill: parent

            onPressed: { dropboxListBody.hide() }
        }

        Button {
            id: dropboxDrop
            height: parent.height
            width: parent.height
            anchors.right: parent.right

            Image {
                source: "images/drop-down.png"
                anchors.centerIn: parent
                smooth: true
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent
                onClicked: { dropboxListBody.toggle() }
            }
        }
    }

    Button {
        id: dropboxListBody

        width: dropboxButtonBody.width
        height: 0
        z: 200

        property int unfoldedHeight: dropboxButtonBody.height * dropboxListSelection.count

        anchors.top: dropboxButtonBody.bottom
        anchors.left: dropboxButtonBody.left

        clip: true
        focus: true

        function toggle()
        {
            if ( height == 0 )
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

        ListView {
            id: dropboxListSelection
            anchors.fill: parent
            currentIndex: 0
            focus: true

            delegate: Item {
                width: dropboxButtonBody.width
                height: dropboxButtonBody.height
                z: 1000

                ButtonText {
                    text: itemText
                    anchors.fill: parent
                    font.bold: ( itemText == selectedText )
                    font.pointSize: 12
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        //                            comboBox.state = ""
                        var prevSelection = selectedText
                        if ( dropboxListSelection.model.get( index ).itemText != prevSelection ) {
                            selectedIndexChanged()
                            console.log( "index is " + index.toString() )
                            dropboxListSelection.currentIndex = index;
                            selectedText = dropboxListSelection.model.get( index ).itemText
                            dropboxListSelection.focus = true
                            dropboxListBody.hide()
                        }

                        comboClicked()
                    }
                }
            }

        }

    }

    MouseArea {
        id: componentMouseArea
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height * ( dropboxListSelection.count + 1 )

        onClicked: {
            console.debug( "componentMouseArea onClicked!!! " )
        }

        hoverEnabled: true
        onExited: { dropboxListBody.hide() }
    }
}



/*
Item {
    width: 400;
    height: 400;

    property string name;
    property string selectedText: listView.model.get( 0 ).itemText
    property alias model: listView.model
    property alias selectedIndex: listView.currentIndex

    signal comboClicked
    signal selectedIndexChanged

    Item {
        id: comboBox

        width: chosenItemText.paintedWidth + 16
        height: 30;
        z: 100;
        smooth: true

        Rectangle {
            id: chosenItem
            radius: 4;
            width: parent.width;
            height: comboBox.height;
            color:  "#222"
            border.color: "black"
            smooth: true

            Text {
                id: chosenItemText
                anchors.centerIn: parent
                anchors.margins: 8;
                text: name
                font.family: "Arial"
                font.pointSize: 14;
                smooth:true
                styleColor: "#000"
                color: "#eee"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    comboBox.state = ( ( comboBox.state === "dropDown" ) ? "" : "dropDown" )
                }
                onExited: {
                    if ( comboBox.state === "dropDown" )
                        comboBox.state = ""
                }
            }
        }

        Rectangle {
            id: dropDown
            width: comboBox.width;
            height: 0;
            clip: true;
            radius: 4;
            anchors.top: chosenItem.bottom;
            anchors.margins: 2;
            color: "#222"
            focus: true


            ListView {
                id: listView
//                anchors.fill: parent
                height:  500//menuItemText.paintedHeight * count
                currentIndex: 0
//                highlight: highlight
                focus: true
                delegate: Item {
                    width: comboBox.width;
                    height: comboBox.height;

                    Text {
                        text: itemText
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.left: parent.left;
                        anchors.margins: 5;
                        font.bold: ( itemText == selectedText )
                        font.pointSize: 12
                        styleColor: "#000"
                        color: "#eee"
                        height: chosenItemText.paintedHeight + 8
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            comboBox.state = ""
                            var prevSelection = selectedText
                            if ( listView.model.get( index ).itemText != prevSelection ) {
                                selectedIndexChanged()
                                console.log("index is " + index.toString() )
                                listView.currentIndex = index;
                                selectedText = listView.model.get( index ).itemText
                                listView.focus = true
                            }

                            comboClicked()
                        }
                    }
                }

                onCurrentIndexChanged: {
                    console.debug( "onCurrentIndexChanged: " + currentIndex.toString() )
                    selectedText = listView.model.get( currentIndex ).itemText
                }

                Keys.onUpPressed: { console.debug("onUpPressed"); if ( selectedIndex < model.count - 1 ) selectedIndex++ }
                Keys.onDownPressed: { console.debug("onDownPressed"); if ( selectedIndex > 0 ) selectedIndex-- }

            }
        }

        MouseArea {
            id: mouseArea
            width: comboBox.width;
            height: dropDown.height
            anchors.top: chosenItem.bottom;
            anchors.margins: 2;

            hoverEnabled: true
            onExited: {
                console.debug( "dropdown mouse area on exited..." )
                comboBox.state = ""
            }
        }

        Component {
            id: highlight
            Rectangle {
                width:comboBox.width;
                height:comboBox.height;
                color: "#222"
                radius: 4
            }
        }

        states: State {
            name: "dropDown";
            PropertyChanges { target: dropDown; height: ( chosenItemText.paintedHeight + 8 ) * listView.count }
            PropertyChanges { target: listView; focus: true }
        }

        transitions: Transition {
            NumberAnimation { target: dropDown; properties: "height"; easing.type: Easing.OutExpo; duration: 1000 }
        }
    }
}
*/
