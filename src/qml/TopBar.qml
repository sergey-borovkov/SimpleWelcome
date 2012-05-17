import QtQuick 1.1

Rectangle {
    id: topBar

    anchors.top: parent.top
    width: parent.width
    height: 80
    //color: "grey"
    color: "transparent"

    Item {
        id: userIconItem
        width: childrenRect.width
        height: childrenRect.height
        
        anchors.verticalCenter: parent.verticalCenter
        x: 16
        
        Image {
            id: userIcon
            source: "image://generalicon/general/usericon"
            width: 64
            height: 64
        }
    }

    Item {
        width: childrenRect.width
        height: childrenRect.height

        anchors.left: userIconItem.right
        anchors.leftMargin: 8

        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: usernameLabel

            maximumLineCount: 1
            elide: Text.ElideRight
            clip: true
            //horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap

            font.pointSize: 18
            font.bold: true
            font.family: "Bitstream Vera Sans"
            style: Text.Raised
            styleColor: "#000"
            color: "#eee"
            

            text: userInfoProvider.userName
        }

    }
    
    Item {
        //color: "white"
        width: 600
        height: 30

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        BorderImage {
            border.left: 10
            border.right: 10
            border.top: 10
            border.bottom: 10
            anchors.fill: parent
            source: "image://generalicon/asset/textfield_border_bg.png"
        }
        
        TextInput {
            //anchors.horizontalCenter: parent.horizontalCenter
            id: searchInput
            width: parent.width - 24            
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            selectByMouse: true
            focus: true
            color: "white"
            font.family: "Bitstream Vera Sans"
            font.italic: true
            font.pixelSize: 18
            
            onTextChanged: {
                searchTextChanged(text);
            }
        }
        Binding {
            target: searchRunner
            property: "searchQuery"
            value: searchInput.text
        }
    }

    DropBox {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16        

        width: 180
        height: 38
    }
    
    /*
    // Old Logout and Shutdown buttons
    Item {
        id: lockButton
        width: childrenRect.width
        height: childrenRect.height
        
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: shutdownButton.left
        //anchors.rightMargin: 8
        anchors.rightMargin: 0

        Image {
            id: lockIcon
            source: "image://generalicon/asset/button_lock.png"
            width: 47
            height: 41
        }

        MouseArea {
            id: lockButtonMouseArea

            anchors.fill: parent
            
            onClicked: {
                sessionProvider.lock();
            }

        }
    }

    Item {
        id: shutdownButton
        width: childrenRect.width
        height: childrenRect.height
        
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16

        Image {
            id: shutdownIcon
            source: "image://generalicon/asset/button_shutdown.png"
            width: 46
            height: 41
        }

        MouseArea {
            id: shutdownButtonMouseArea

            anchors.fill: parent
            
            onClicked: {
                sessionProvider.shutdown();
            }

        }
    }
    */

    function searchTextChanged(text)
    {
        if(text.length <= 0)
          searchTab.hideSearchTab();
    }
}
