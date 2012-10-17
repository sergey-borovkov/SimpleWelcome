import QtQuick 1.1
import Private 0.1

Item {
    id: cloudRect
    property string id
    property date cloudDate
    property variant model
    property int like: 0
    property int likes: 0
    property int commentCount: 0
    property string message: ""
    property string picture: ""
    property string pluginName: ""
    property alias iconPlugin : iconPlugin
    property alias commentsView : commentsListView

    MouseArea {
        id: modal
        anchors.fill: parent
        onClicked:  {
            cloudRect.state = ""
        }
        enabled: false
    }

    WheelArea {
        id: modalWheelArea
        anchors.fill: parent
        enabled: false
    }

    BorderImage {
        id: innerShadow
        anchors.fill: parent
        border { left: 23; top: 23; right: 23; bottom: 23 }
        source: "images/shadow-inverse.png"
        smooth: true
    }

    Item {
        id: socialCloudItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height

        objectName: "SocialCloudItem"
        property Item mainParent: cloudRect

        ItemRectangle
        {
            id: mainRect
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height

            Rectangle {
                id: topLine
                color: "transparent"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: 26

                Image {
                    id: iconPlugin
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    width: 22
                    height: 22
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    //source: iconPlugin.source
                }
                Item {
                    id: likesCountArea
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.leftMargin: 5
                    width: 40
                    height: 22
                    Image {
                        id: smallLikeIcon
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/like.png"
                    }
                    Text {
                        anchors.left: smallLikeIcon.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: likes
                        color: "white"
                    }
                }
                Item {
                    id: commentsCountArea
                    anchors.right: likesCountArea.left
                    anchors.rightMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    width: 40
                    height: 22
                    Image {
                        id: smallCommentIcon
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/comment.png"
                    }
                    Text {
                        anchors.left: smallCommentIcon.right
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        anchors.leftMargin: 5
                        text: commentCount
                        color: "white"
                    }
                }

                Item {
                    id: detailsOffArea
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    width: 12
                    height: 12
                    visible: false
                    Image{
                        id: closeIcon
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/x.png"
                        width: 8
                        height: 8
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            modal.parent = cloudRect
                            modal.z = -1
                            cloudRect.state = ""
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
                color: "transparent"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                height: 10
                Item {
                    id: bottomCommentsCountArea
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 40
                    height: 22
                    visible: false
                    Image {
                        id: bottomCommentIcon
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/comment.png"
                    }
                    Text {
                        anchors.left: bottomCommentIcon.right
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        anchors.leftMargin: 5
                        text: commentCount
                        color: "white"
                    }
                }
                Item {
                    id: bottomLikesCountArea
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: bottomCommentsCountArea.right
                    anchors.leftMargin: 5
                    width: 40
                    height: 22
                    visible: false
                    Image {
                        id: bottomLikeIcon
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/like.png"
                    }
                    Text {
                        anchors.left: bottomLikeIcon.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: likes
                        color: "white"
                    }
                }

                Item {
                    id: likeItemArea
                    anchors.right:  parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 10
                    height: parent.height
                    width: 100
                    visible: false
                    state: (like == 1)? "liked" : ""
                    Text {
                        id: likesText
                        anchors.fill: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: i18n_Like
                        color: "white"
                    }
                    MouseArea{
                        id: likeSendArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: likesText.font.bold = true
                        onExited: likesText.font.bold = false
                        onClicked: {
                            if (likeItemArea.state === "") {
                                socialProxy.likeItem(id, pluginName);
                            } else {
                                socialProxy.unlikeItem(id, pluginName);
                            }
                        }
                    }
                    states: [
                        State {
                            name: "liked"
                            PropertyChanges { target: likesText; text: i18n_Unlike }
                            //PropertyChanges { target: likeSendArea; enabled: false }
                        }
                    ]
                }
                Item {
                    id: commentsShowArea
                    anchors.centerIn: parent
                    width: 140
                    height: parent.height
                    visible: false
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: "images/arrow.png"
                    }
                    Text {
                        id: commentsShowAreaText
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                        text: (cloudRect.state === "comments") ? i18n_Hide_Comments : i18n_Show_Comments
                    }
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        source: "images/arrow.png"
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: commentsShowAreaText.font.bold = true
                        onExited: commentsShowAreaText.font.bold = false
                        onClicked: {
                            if (cloudRect.state === "details") {
                                //Set source on comments loader
                                commentsEdit.source = "CommentsEditBox.qml"
                                commentsEdit.item.edit.color = "grey"
                                commentsEdit.item.edit.text = i18n_Write_Comment
                                commentsEdit.item.userPhoto.source = socialProxy.selfPictureUrl()
                                commentsListView.view.positionViewAtEnd()
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
            }
        }
        ItemRectangle {
            id: commentsRect
            height: 0
            anchors.top: mainRect.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors { leftMargin: 30; topMargin: -10; rightMargin: 30; bottomMargin: 8}
            z: -1

            CommentsListView {
                id: commentsListView
                anchors.top : parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: commentsEdit.top
                anchors { topMargin: 15; leftMargin: 3; rightMargin: 3; bottomMargin: 5}
                visible: false
            }

            Loader {
                id: commentsEdit
                height: 0
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.rightMargin: 2
                anchors.bottomMargin: 2
                anchors.right: parent.right
                visible: false
            }
            Connections {
                target: commentsEdit.item
                onSend: {
                    socialProxy.commentItem(comment, id, pluginName)
                }
            }
        }
        MouseArea
        {
            id: detailsOnArea
            anchors.fill: parent
            z:-2
            onClicked: {
                if (cloudRect.state === "") {
                    modal.parent = timeFrameTab;
                    modal.z = 300
                    cloudRect.state = "details"
                    //Query all comments and likes
                    socialProxy.getAllComments(id, pluginName)
                    socialProxy.getAllLikes(id, pluginName)
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

            AnchorChanges {
                target: socialImage
                anchors.left : undefined
                anchors.right : undefined
                anchors.horizontalCenter : bodyItem.horizontalCenter
                anchors.top : bodyItem.top
                anchors.bottom : undefined
                anchors.verticalCenter : undefined
            }

            AnchorChanges {
                target: socialMessage
                anchors.left : undefined
                anchors.right : undefined
                anchors.horizontalCenter : bodyItem.horizontalCenter
                anchors.top : socialImage.bottom
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
            }

            ParentChange { target: modalWheelArea; parent: timeFrameTab }

            PropertyChanges { target: socialCloudItem; z: 400 }

            PropertyChanges { target: socialMessage; text: message }

            PropertyChanges { target: likesCountArea; visible: false }

            PropertyChanges { target: commentsCountArea; visible: false }

            PropertyChanges { target: bottomLikesCountArea; visible: true }

            PropertyChanges { target: bottomCommentsCountArea; visible: true }

            PropertyChanges { target: detailsOffArea; visible: true }

            PropertyChanges { target: commentsShowArea; visible: true }

            PropertyChanges { target: likeItemArea; visible: true}

            PropertyChanges { target: bottomLine; height: Math.max( 26, commentsShowAreaText.paintedHeight ); visible: true }

            PropertyChanges { target: modal; enabled: true }

        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: socialCloudItem; height: 300 + commentsViewHeight() + 60 + 10}
            PropertyChanges { target: mainRect; height: 300 }
            PropertyChanges { target: commentsRect; height: commentsViewHeight() + 60 + 10}

            PropertyChanges {
                target: commentsListView
                visible: true
                height: commentsViewHeight()
            }

            PropertyChanges { target: commentsEdit; height: 60; visible: true }
        }
    ]

    transitions: [
        Transition {
            ColorAnimation { property: "color"; duration: 300 }
            AnchorAnimation { duration: 300 }
            ParentAnimation {
                via: foreground
                NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }

            }
        }
    ]

    function textVisible()
    {
        if (picture === "")
        {
            socialMessage.text = ""
            return false
        }
        return true
    }

    function commentsViewHeight() {
        if (commentCount < 3)
            return commentCount * 60
        else
            return 180
    }
}


