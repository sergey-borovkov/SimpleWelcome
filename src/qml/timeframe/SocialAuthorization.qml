import QtQuick 1.1
import ".."

ListView {
    spacing: 15
    delegate: Item {
        //width: Math.max(img.width,  ) + 10
        width: childrenRect.width
        height: img.height + button.height + 10
        Image {
            // get icon for this plugin
            id: img
            smooth: true
            z: -1
            width: 128
            height: 128
            source: "image://plugin/" + name
        }
        Item {
            id: button
            anchors.top: img.bottom
            anchors.left: img.left
            anchors.right: img.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            height: 28
            Image {
                id: buttonImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: authorized ? 15 : 20
                width: 22
                height: 22
                smooth: true
                source: authorized ? "images/logout.png" : "images/login.png"

            }
            Text {
                id: buttonText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: buttonImage.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                text: authorized ? "Logout" : "Login"
                color: "white"
                font.pointSize: 12
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                img.width = 134
                img.height = 134
                buttonText.font.bold = true
                buttonImage.height = 24
                buttonImage.width = 24
            }
            onExited: {
                img.width = 128
                img.height = 128
                buttonText.font.bold = false
                buttonImage.height = 22
                buttonImage.width = 22
            }

            onClicked: {
                if(authorized) {
                    socialProxy.logout(name)
                }
                else {
                    socialProxy.login(name)
                }
            }
        }
    }
}
