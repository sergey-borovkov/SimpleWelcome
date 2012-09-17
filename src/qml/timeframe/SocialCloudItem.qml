import QtQuick 1.1
Item{
    id: cloudRect
    property string id
    property date cloudDate
    property variant model
    property int like: 0
    property int likes: 0
    property int comments: 0
    property string message: ""
    property string picture: ""
    property string pluginName: ""
    property alias pluginIcon : iconPlugin
    property alias commentsView : commentsListView
    MouseArea {
        id: modal
        anchors.fill: parent
    }

    BorderImage {
        id: innerShadow
        anchors.fill: parent
        border { left: 23; top: 23; right: 23; bottom: 23 }
        source: "images/inner-shadow.png"
        smooth: true
        //visible: false
        opacity: 0
    }

    Item {
        id: socialCloudItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height

        BorderImage {
            id: shadowBorder
            anchors.fill: parent
            anchors { leftMargin: -23; topMargin: -23; rightMargin: -23; bottomMargin: -23 }
            border { left: 23; top: 23; right: 23; bottom: 23 }
            source: "images/shadow2.png"
            smooth: true
        }
        Rectangle
        {
            anchors.centerIn: parent
            id: background
            //property string pluginName : "VKontakte"
            //color: "#aa5f5f5f"
            color: "transparent"
            border.color: "#5f5f5f"
            border.width: 0
            //radius: 10
            clip: true
            width: parent.width
            height: parent.height
            //Column{
            //  width: parent.width
            Rectangle {
                id: topLine
                border.color: "#335f5f5f"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: 0
                visible: false

                Image {
                    id: topPluginIcon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 20
                    height: 20
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: iconPlugin.source
                }
                Item {
                    id: detailsOffArea
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    width: 20
                    height: 20
                    Image{
                        id: closeIcon
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/window-close.png"
                        width: 16
                        height: 16
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            modal.parent = cloudRect
                            cloudRect.state = ""
                        }
                        onEntered: {
                            closeIcon.width = 18
                            closeIcon.height = 18
                        }
                        onExited:{
                            closeIcon.width = 16
                            closeIcon.height = 16
                        }

                    }
                }
            }
            Item {
                id: bodyItem
                anchors.top: topLine.bottom
                anchors.bottom: bottomLine.top
                width: parent.width
                clip: true
                Text {
                    id: socialMessage
                    width: parent.width
                    //anchors.centerIn: parent
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: (picture === "") ? message : ""
                    color: "white"
                    maximumLineCount: 5
                    elide: Text.ElideRight
                    //visible: textVisible()
                }
                Image {
                    id: socialImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: picture
                    width: Math.min( sourceSize.width, parent.width)
                    height: Math.min( sourceSize.height, parent.height)
                    clip: true
                }
            }
            Rectangle {
                id: bottomLine
                color: "#335f5f5f"
                anchors.bottom: commentsListView.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: 26

                Text {
                    id: likesCount
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    text: "Likes: " + likes
                    color: "grey"
                    visible: false
                }
                Item {
                    id: likeItem
                    anchors.centerIn: parent
                    height: parent.height
                    width: 30
                    visible: false
                    state: (like == 1)? "liked" : ""
                    Text {
                        id: likesText
                        anchors.fill: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "Like"
                        color: "grey"
                    }
                    MouseArea{
                        id: likeSendArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: likesText.font.bold = true
                        onExited: likesText.font.bold = false                        
                        onClicked: {
                            if (likeItem.state === "") {
                                console.log("add like to item, item id: " + id)
                                socialProxy.likeItem(id, pluginName);
                            } else {
                                console.log("dislike item, item id: " + id)
                                socialProxy.dislikeItem(id, pluginName);
                            }
                        }
                    }
                    states: [
                        State {
                            name: "liked"
                            PropertyChanges { target: likesText; text: "Dislike" }
                            //PropertyChanges { target: likeSendArea; enabled: false }
                        }
                    ]
                }
                Item {
                    id: commentsCount
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right:  parent.right
                    anchors.rightMargin: 10
                    width: 100
                    height: parent.height
                    visible: false

                    Text {
                        anchors.fill: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "Comments: " + comments
                        color: "grey"

                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if (cloudRect.state === "details") {
                                //Set source on comments loader
                                commentsEdit.source = "CommentsEditBox.qml"
                                commentsEdit.item.edit.color = "grey"
                                commentsEdit.item.edit.text = "Write comment..."
                                cloudRect.state = "comments"
                            }
                            else if (cloudRect.state === "comments")
                            {
                                commentsEdit.source = ""
                                cloudRect.state = "details"
                            }
                        }
                    }
                }
                Image {
                    id: likeIcon
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    width: 16
                    height: 16
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/thumb-up.png"
                }

                Image {
                    id: iconPlugin
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: 16
                    height: 16
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    //source: "image://plugin/" + pluginName
                }

                Image {
                    id: commentIcon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right:  parent.right
                    anchors.rightMargin: 10
                    width: 16
                    height: 16
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/comment.png"
                }
            }

            ListView {
                id: commentsListView
                //anchors.top : bottomLine.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: commentsEdit.top
                clip: true
                visible: false
                snapMode:  ListView.SnapToItem
                delegate: Item {
                    width: 200; height: 60
                    Image
                    {
                        id: userPhoto
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                        width: 55
                        anchors.rightMargin: 5
                        source: "images/user.png"
                    }
                    Text {
                        id: nameField;
                        anchors.left: userPhoto.right
                        anchors.top: parent.top
                        anchors.leftMargin: 10
                        text: from
                    }
                    Text {
                        id: textField;
                        anchors.left: userPhoto.right
                        anchors.top: nameField.bottom
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 10
                        text: messageText
                        elide: Text.ElideRight
                    }
                }
            }
            Loader {
                id: commentsEdit
                height: 0
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                visible: false
            }
            Connections {
                target: commentsEdit.item
                onSend: {
                    console.log("comment sending via text edit: " +comment)
                    socialProxy.commentItem(comment, id, pluginName)
                }
            }

        }


        MouseArea
        {
            id: detailsOnArea
            anchors.fill: parent
            z: 9999
            onClicked: {
                if (cloudRect.state === "") {
                    cloudRect.state = "details"
                    modal.parent = timeFrameTab;
                }
                else {
                    modal.parent = socialCloudItem
                    cloudRect.state = ""
                }
            }
        }
    }

    states: [
        State {
            name: "details"

            AnchorChanges {
                target: socialCloudItem
                anchors.left : undefined
                anchors.right : undefined
                anchors.horizontalCenter : undefined
                anchors.top : undefined
                anchors.bottom : undefined
                anchors.verticalCenter : undefined
            }

            ParentChange {
                target: socialCloudItem
                parent: timeFrameTab
                x: timeFrameTab.width/2 - socialCloudItem.width/2
                y: timeFrameTab.height/2 - socialCloudItem.height/2
                width: 400
                height: 300
            }/*
            AnchorChanges {
                target: socialMessage
                anchors.verticalCenter: undefined
                anchors.top: bodyItem.top
                anchors.horizontalCenter: bodyItem.horizontalCenter
            }
            AnchorChanges {
                target: socialImage
                anchors.verticalCenter: undefined
                anchors.top: socialMessage.bottom
                anchors.horizontalCenter: bodyItem.horizontalCenter
                anchors.bottom: bodyItem.bottom
            }*/
            PropertyChanges {
                target: topLine
                height: 26
                visible: true
            }

            PropertyChanges { target: socialMessage; text: message }

            PropertyChanges { target: background; color: "white" }

            PropertyChanges { target: socialMessage; color: "black" }

            PropertyChanges { target: likesCount; visible: true }

            PropertyChanges { target: commentsCount; visible: true }

            PropertyChanges { target: likeIcon; visible: false }

            PropertyChanges { target: iconPlugin; visible: false }

            PropertyChanges { target: commentIcon; visible: false }

            PropertyChanges { target: detailsOnArea; enabled: false }

            PropertyChanges { target: innerShadow; opacity: 1 }

            PropertyChanges { target: likeItem; visible: true}
        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: socialCloudItem; height: 300 + 60 * (commentsListView.model.rowCount() + 1) }

            PropertyChanges {
                target: commentsListView
                visible: true
                height: 60 * commentsListView.model.rowCount()
            }

            PropertyChanges { target: commentsEdit; height: 60; visible: true }
        }
    ]

    transitions: [
        Transition {
            ColorAnimation { property: "color"; duration: 300 }
            //AnchorAnimation { duration: 300 }
            ParentAnimation {
                via: foreground
                NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }

            }
        }
    ]

    function textVisible()
    {
        console.log(picture)
        if (picture === "")
        {
            socialMessage.text = ""
            return false
        }
        console.log("true")
        return true
    }
}


