import QtQuick 1.1
import Private 0.1

Item {
    id: cloudRect
    objectName: "cloudRect"
    property string id
    property date cloudDate
    property variant model
    property int like: 0
    property int likes: 0
    property int commentCount: 0
    property string message: ""
    property string picture: ""
    property string pluginName: ""
    property string audio: ""
    property alias iconPlugin : iconPlugin
    property alias commentsView : commentsListView
    property int index: -1

    function update() {
        if(index === -1) {
            return;
        }

        likes = model.likesCount(index)
        commentCount = model.commentsCount(index)
        like = model.like(index)
    }

    function initialize(index)
    {
        id = model.id(index)
        message = model.text(index)
        picture = model.imageUrl(index)
        audio = model.audio(index)
        like = model.like(index)
        likes = model.likesCount(index)
        commentCount = model.commentsCount(index)
        pluginName = model.pluginName(index)
        iconPlugin.source =  "image://plugin/" + pluginName + "/small"
        commentsView.model = model.comments(index)
    }

    function popupDetailsWidget()
    {
        if (cloudRect.state === "") {
            modal.parent = timeFrameTab;
            modal.z = 300
            cloudRect.state = "details"
            socialProxy.getAllComments(id, pluginName)
            socialProxy.getAllLikes(id, pluginName)
        }
    }

    function msgViewHeight() {
        var h = mainRect.height - bottomLine.height - topLine.height - 10
        if (socialImage.height)
            h = h - socialImage.height - 10
        if (audioItem.visible)
            h = h - audioItem.height - 10;
        return Math.min(socialMessage.height, h)
    }

    MouseArea {
        id: modal
        anchors.fill: parent
        onClicked:  {
            cloudRect.state = ""
        }
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

        ItemRectangle {
            id: mainRect
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height

            Column {
                id: column
                width: parent.width
                anchors.bottomMargin: 5
                anchors.top: parent.top
                anchors.bottom: bottomLine.top
                spacing: 10

                Rectangle {
                    id: topLine
                    color: "transparent"
                    //                    anchors.top: parent.top
                    //                    anchors.right: parent.right
                    //                    anchors.left: parent.left
                    height: 26
                    width: parent.width

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
                    width: parent.width
                    height: socialImage.height

                    Image { //Main image
                        id: socialImage
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.PreserveAspectFit
                        height: Math.min( sourceSize.height, mainRect.height - topLine.height - 20)
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        smooth: true
                        source: picture
                        clip: true
                    }
                }

                Text {
                    function getAudio()
                    {
                        if(typeof audio === "undefined")
                            return ""
                        else
                            return i18n_Audio + audio
                    }

                    id: audioItem
                    width: parent.width - 20
                    anchors.bottomMargin: 3
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    maximumLineCount: 2
                    text: getAudio()
                    color: "lightblue"
                    visible: (picture === "") && (audio !== "")
                }

                Item {
                    id: msgViewRect
                    width: parent.width
                    height: (picture === "") ? msgView.height : 0

                    Flickable {
                        id: msgView
                        width: parent.width
                        height: socialMessage.height
                        contentHeight: socialMessage.paintedHeight
                        clip: true

                        Text {
                            id: socialMessage
                            width: msgView.width - 20
                            anchors.bottomMargin: 3
                            anchors.horizontalCenter: parent.horizontalCenter
                            wrapMode: Text.Wrap
                            horizontalAlignment: truncated ? Text.AlignLeft : Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: (picture === "") ? message : ""
                            visible: (picture === "")

                            color: "white"
                            clip: true
                            textFormat: Text.StyledText
                            elide: Text.ElideRight
                            maximumLineCount: { var lines = ( mainRect.height - topLine.height - (audioItem.visible ? audioItem.height : 0)) / font.pixelSize / 1.5; return lines }

                            onLinkActivated: {
                                Qt.openUrlExternally(link)
                            }

                            MouseArea {
                                id: msgMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    popupDetailsWidget()
                                }
                            }
                        }
                    }
                    ScrollBar {
                        id: msgScrollBar
                        flickable: msgView
                        vertical: true
                        hideScrollBarsWhenStopped: false
                        visible: false
                    }
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
                                commentsEdit.item.userPhoto.source = socialProxy.selfPictureUrl(pluginName)
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
                anchors.leftMargin: 1 //3
                anchors.rightMargin: 1 //3
                anchors.right: parent.right
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
            z: -2
            onClicked: {
                //console.log("details on")
                popupDetailsWidget()
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
            }

            PropertyChanges { target: timeFrameTab; enableWheel: false }

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

            PropertyChanges { target: socialMessage; textFormat: Text.RichText; visible: true; horizontalAlignment: (msgView.contentHeight > msgView.height) ? Text.AlignLeft : Text.AlignHCenter }

            PropertyChanges { target: audioItem; visible: audio !== "" }

            PropertyChanges { target: msgMouseArea; enabled: false; z: -1 }

            PropertyChanges { target: msgView; height: msgViewHeight() }

            PropertyChanges { target: msgScrollBar; visible: (msgView.contentHeight > msgView.height) }

            PropertyChanges { target: cloudRect; z: 9000; height: { var y = cloudRect.height; return y } }

            PropertyChanges { target: msgViewRect; height: msgView.height }

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
