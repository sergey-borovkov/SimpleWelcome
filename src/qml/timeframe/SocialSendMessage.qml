import QtQuick 1.1
import Private 0.1
import ".."

Item{
    id: socialSendMessageRect

    x: sendMessageButton.x - 60
    y: sendMessageButton.y + sendMessageButton.height

    width: wndWidth
    height: 0
    opacity: 0
    clip: true
    z: 500

    objectName: "sendMessageItem"
    property Item mainParent: socialSendMessageRect

    property int wndWidth: 500
    property int wndHeight: 300
    property int networkIconWidth: 22
    property int networkIconHeight: 22
    property int moveDuration: 150
    property int animationDuration: 150

    function popupSendMessageWidget()
    {
        if (socialSendMessageRect.state === "") {
            socialSendMessageRect.state = "showSendMessageWindow"
        }
    }

    function hideSendMessageWidget()
    {
        modal.z = -1
        socialSendMessageRect.state = ""
    }

    function trim(str) {
        return str.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
    }

    function sendMessage()
    {
        var msg = edit.text

        // exit if empty message
        if (edit.pressedKey && msg.length != 0 && msg.replace(/\s/g,"") != "") {

            if (msg.length > 4096)
                msg = msg.slice(0, 4096)

            // trim spaces from start and end of string
            msg = trim(msg)
            // send MSG....
            console.log("Send message to " + socialNetworksView.currentItem.pluginName)
            socialProxy.postMessage(msg, socialNetworksView.currentItem.pluginName)
        }

        hideSendMessageWidget()
    }

    function findCurrentPlugin()
    {
        var idx = 0
        for (var i = 0; i < socialNetworksView.count; i++) {
            if (socialNetworksView.model.isAuthorized(i)) {
                idx = i
                break
            }
        }

        return idx
    }

    MouseArea {
        id: modal
        anchors.fill: parent
        enabled: false
        onClicked:  {
            hideSendMessageWidget()
        }
    }

    BorderImage {
        id: innerShadow
        anchors.fill: parent
        source: "images/write-message-body.png"
        anchors { topMargin: -36; leftMargin: -11 }
        border { left: 124; top: 56; right: 49; bottom: 47 }
        smooth: true
        z: -1
    }

    Item {
        id: sendMessageItem
        anchors { fill: parent; topMargin: 24; leftMargin: 40; rightMargin: 54; bottomMargin: 48 }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            z: -2
        }

        // label
        Label {
            id: label
            text: i18n("Write message")
            horizontalAlignment: Text.AlignLeft

            anchors {
                top: parent.top
                left: parent.left
                right: windowCloseButton.left
            }
        }

        // close button
        Item {
            id: windowCloseButton

            anchors {
                verticalCenter: label.verticalCenter
                right: parent.right
            }
            width: 10
            height: label.height

            Image{
                id: closeIcon

                anchors.centerIn: parent

                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "images/x.png"
                width: 8
                height: 8
            }

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true
                onClicked: {
                    hideSendMessageWidget()
                }
                onEntered: {
                    closeIcon.width = 10
                    closeIcon.height = 10
                }
                onExited:{
                    closeIcon.width = 8
                    closeIcon.height = 8
                }
            }
        }

        // edit field
        Rectangle {
            id: editBorder

            anchors {
                top: label.bottom
                bottom: sendButton.top
                left: parent.left
                right: parent.right
                topMargin: 7
                bottomMargin: 10
            }

            color: "white"
            border.color: "#BDC7D8"

            Flickable {
                id: flick
                anchors {
                    fill: parent
                    topMargin: 3
                    leftMargin: 3
                    rightMargin: 3
                    bottomMargin: 3
                }
                contentWidth: edit.paintedWidth
                contentHeight: edit.paintedHeight
                clip: true


                function ensureVisible(r)
                {
                    if (contentX >= r.x)
                        contentX = r.x;
                    else if (contentX+width <= r.x+r.width)
                        contentX = r.x+r.width-width;
                    if (contentY >= r.y)
                        contentY = r.y;
                    else if (contentY+height <= r.y+r.height)
                        contentY = r.y+r.height-height;
                }

                TextEdit {
                    id: edit
                    width: flick.width
                    height: flick.height
                    focus: true
                    wrapMode: TextEdit.Wrap
                    onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                    text: i18n("What do you think about?")
                    color: "grey"
                    cursorPosition: 0
                    clip: true
                    visible: false

                    property bool pressedKey: false

                    Keys.onPressed: {
                        if (edit.text === i18n("What do you think about?"))
                        {
                            edit.color = "black"
                            edit.text = ""
                            edit.cursorPosition = 0
                            pressedKey = true
                        }
                        if (event.key === Qt.Key_Return) {

                            // add "shift+enter" process
                            if (event.modifiers & Qt.ShiftModifier) {
                                return;
                            }

                            // send message to network
                            sendMessage()

                            event.accepted = true
                        }
                    }
                }
            }
        }


        Component {
            id: pluginDelegate

            Item {
                id: wrapper
                width: authorized ? pluginImage.width + 10 : 0
                height: pluginImage.height
                visible: authorized

                property string pluginName: name
                property bool pluginAuthorized: authorized

                Image {
                    id: pluginImage
                    anchors.left: parent.left
                    width: authorized ? networkIconWidth : 0
                    height: authorized ? networkIconHeight : 0
                    fillMode: Image.PreserveAspectFit
                    source: "image://plugin/" + name + "/small"
                    smooth: true
                    opacity: 0.5
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        socialNetworksView.currentIndex = index
                    }
                }

                // change opacity of the item if it is the current item
                states: State {
                    name: "Current"
                    when: wrapper.ListView.isCurrentItem
                    PropertyChanges { target: pluginImage; opacity: 1 }
                }
                transitions: Transition {
                    NumberAnimation { properties: "opacity"; duration: 200 }
                }
            }
        }

        // plugin buttons
        ListView {
            id: socialNetworksView
            anchors {
                verticalCenter: sendButton.verticalCenter
                left: parent.left
                right: sendButton.left
            }
            height: networkIconHeight
            orientation: ListView.Horizontal
            interactive: false
            focus: true

            model: pluginModel
            delegate: pluginDelegate
        }

        // "send" button
        Item {
            id: sendButton
            width: 80
            height: 0

            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            Item {
                id: sendTabButtonBg
                anchors.fill: parent

                property alias pressedAndHovered: sendTabButton.pressedAndHovered

                BorderImage {
                    border.left: 6
                    border.right: 7
                    border.top: 0
                    border.bottom: 0
                    anchors.fill: parent
                    anchors.rightMargin: -3
                    source: sendTabButtonBg.pressedAndHovered ? "image://generalicon/asset/tab_button_pressed.png" : "image://generalicon/asset/tab_button.png"
                }

                Behavior on x {
                    NumberAnimation { duration: 200; }
                }
            }

            TabButton {
                id: sendTabButton
                anchors.fill: parent
                label: i18n("Send")
                pressable: true
                active: true

                onButtonClick: {
                    // send message...
                    sendMessage()
                }
            }
        }
    }

    states: [
        State {
            name: "showSendMessageWindow"

            PropertyChanges {
                target: socialSendMessageRect;
                opacity: 1
                width: wndWidth
                height: wndHeight
                clip: false
                x: sendMessageButton.x - 60
            }

            ParentChange { target: modal; parent: timeFrameTab }

            PropertyChanges { target: modal; enabled: true; z: 300 }

            PropertyChanges { target: timeFrameTab; enableWheel: false }

            PropertyChanges { target: sendButton; height: 27 }

            PropertyChanges {
                target: edit;
                text: i18n("What do you think about?");
                color: "grey";
                cursorPosition: 0;
                visible: true;
                pressedKey: false
            }

            PropertyChanges { target: socialNetworksView; currentIndex: findCurrentPlugin() }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }
        }
    ]

}
