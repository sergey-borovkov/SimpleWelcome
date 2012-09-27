import QtQuick 1.1
import ".."

PathView {
    anchors.fill: parent
    path: Path {
        startX: 0
        startY: height/2
    }

    delegate: Item {

        anchors.fill: parent
        Item {
            id: authorizationItem
            //width: Math.max(img.width,  ) + 10
            width: childrenRect.width
            height: img.height + button.height + 10
            anchors.centerIn: parent

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
                    text: authorized ? i18n_Login : i18n_Logout
                    color: "white"
                    font.pointSize: 12
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: authorizationItem.state = "hover"
                onExited:  authorizationItem.state = ""
                onPressed: authorizationItem.state = ""
                onReleased: authorizationItem.state = "hover"
                onClicked: {
                    if(authorized) {
                        socialProxy.logout(name)
                    }
                    else {
                        socialProxy.login(name)
                    }
                }
            }
            states: [
                State {
                    name: "hover"
                    PropertyChanges { target: img; width: 134; height: 134 }
                    PropertyChanges { target: buttonImage; width: 24; height: 24 }
                }
            ]
            transitions: [
                Transition {
                    NumberAnimation { properties: "width,height"; duration: 100}
                }
            ]
        }
    }
}
