import QtQuick 1.1
import Private 0.1
import ".."

Item{
    id: socialSendMessageRect

    x: sendMessageButton.x
    y: sendMessageButton.y + sendMessageButton.height + 10
    width: 0
    height: 0
    opacity: 0

    objectName: "sendMessageItem"
    property Item mainParent: socialSendMessageRect

    property int wndWidth: 500
    property int wndHeight: 300
    property int networkIconWidth: 22
    property int networkIconHeight: 22

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
        console.log("!!!!!   SEND button clicked...")

        var msg = edit.text

        // exit if empty message
        if (msg.length != 0 && msg.replace(/\s/g,"") != "") {

            if (msg.length > 4096)
                msg = msg.slice(0, 4096)

            // trim spaces from start and end of string
            msg = trim(msg)
            // send MSG....
            for (var i=0; i<networkButtonsRepeater.count; i++) {
                if (networkButtonsRepeater.itemAt(i).pluginChecked) {
                    console.log("Plugin " + networkButtonsRepeater.itemAt(i).pluginName + " is checked - post message \"" + msg + "\"")
                    socialProxy.postMessage(msg, networkButtonsRepeater.itemAt(i).pluginName)
                }
            }
        }

        hideSendMessageWidget()
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
        border { left: 23; top: 23; right: 23; bottom: 23 }
        source: "images/shadow-inverse.png"
        smooth: true
    }

    ItemRectangle {
        id: sendMessageItem
        anchors.fill: parent

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
                topMargin: 10
                leftMargin: 10
                rightMargin: 10
            }
        }

        // close button
        Item {
            id: windowCloseButton

            anchors {
                verticalCenter: label.verticalCenter
                right: parent.right
                rightMargin: 5
            }
            width: 12
            height: 12

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
                topMargin: 10
                bottomMargin: 10
                leftMargin: 10
                rightMargin: 10
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

                    Keys.onPressed: {
                        if (edit.text === i18n("What do you think about?"))
                        {
                            edit.color = "black"
                            edit.text = ""
                            edit.cursorPosition = 0
                        }
                        if (event.key === Qt.Key_Return) {

                            // add "shift+enter" process
                            if (event.modifiers & Qt.ShiftModifier) {
                                return;
                            }

                            // send message to network
                            sendMessage()

//                            edit.color = "grey"
//                            edit.text = i18n("What do you think about?")
                            event.accepted = true
                        }
                    }
                }
            }
        }

        // plugin buttons
        Row {
            id: socialNetworksView
            spacing: 10
            anchors {
                verticalCenter: sendButton.verticalCenter
                left: parent.left
                leftMargin: 10
            }

            Repeater {
                id: networkButtonsRepeater
                model: pluginModel

                Item {
                    width: networkIconWidth
                    height: networkIconHeight

                    property bool pluginChecked: true
                    property string pluginName: name

                    Image {
                        id: iconPluginButton
                        anchors.centerIn: parent
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: "image://plugin/" + name + "/small"
                        smooth: true
                        visible: authorized
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            pluginChecked = !pluginChecked
                            iconPluginButton.opacity = (pluginChecked ? 1 : 0.5)
                            console.log("button of " + name + "pressed...")
                        }
                    }
                }
            }
        }

        // "send" button
        Item {
            id: sendButton
            width: 80
            height: 27

            anchors {
                right: parent.right
                bottom: parent.bottom
                rightMargin: 10
                bottomMargin: 10
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
                z: 500
                opacity: 1
                width: wndWidth
                height: wndHeight
                x: galleryButton.x
                y: sendMessageButton.y + sendMessageButton.height + 10
            }

            ParentChange { target: modal; parent: timeFrameTab }

            PropertyChanges { target: modal; enabled: true; z: 300 }

            PropertyChanges { target: timeFrameTab; enableWheel: false }

            PropertyChanges { target: edit; text: i18n("What do you think about?"); color: "grey"; cursorPosition: 0 }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }
        }
    ]

}
