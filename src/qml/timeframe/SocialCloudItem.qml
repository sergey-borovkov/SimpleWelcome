import QtQuick 1.1
import Private 0.1

Item {
    id: cloudRect
    objectName: "cloudRect"
    property string id
    property date datetime
    property date cloudDate
    property variant model
    property int like: 0
    property int likes: 0
    property int commentCount: 0
    property string message: ""
    property string picture: ""
    property string pluginName: ""
    property string audio: ""
    property string audioUrl: ""
    property string video: ""
    property string videoUrl: ""
    property string fromName: ""
    property string fromImageUrl: ""
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
        datetime = model.datetime(index)
        picture = model.imageUrl(index)
        audio = model.audio(index)
        audioUrl = model.audioUrl(index)
        video = model.video(index)
        videoUrl = model.videoUrl(index)
        like = model.like(index)
        likes = model.likesCount(index)
        fromName = model.fromName(index)
        fromImageUrl = model.fromImageUrl(index)
        commentCount = model.commentsCount(index)
        pluginName = model.pluginName(index)
        topLine.pluginIcon =  "image://plugin/" + pluginName + "/small"
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
        var h = mainRect.height - bottomLine.height - topLine.height - fromItem.height - 10
        if (socialImage.height)
            h = h - socialImage.height - 10
        if (audioItem.visible)
            h = h - audioItem.height - 10;
        if (videoItem.visible)
            h = h - videoItem.height - 10;
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

            SocialTopBar {
                id: topLine

                anchors.top: parent.top
                width: parent.width
                height: 26

                dateText : date
                likesCount : likes
                commentsCount: commentCount

                onExitClicked: {
                    modal.parent = cloudRect
                    modal.z = -1
                    cloudRect.state = ""
                }
            }

            FromItem {
                id: fromItem
                userName: fromName
                userImage: fromImageUrl
                width: parent.width - 20
                anchors { top: topLine.bottom; bottomMargin: 10 }
            }

            Item{
                id: bodyItem

                anchors { top: fromItem.bottom; bottom: bottomLine.top; left: parent.left; right: parent.right }

                Column {
                    id: column

                    anchors.centerIn: parent
                    width: parent.width

                    spacing: 10

                    Item {
                        id: imageAnchor
                        width: parent.width
                        height: socialImage.height

                        SocialImage { //Main image
                            id: socialImage
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                                leftMargin: 5
                                rightMargin: 5
                            }
                            width: getWidth(); height: getHeight()

                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            clip: true
                            source: picture
                            function getWidth() {
                                if (status === Image.Null)
                                    return 0
                                if ((status===Image.Loading) || (status===Image.Error))
                                    return 55
                                return Math.min( sourceSize.width, mainRect.width - 20)
                            }
                            function getHeight() {
                                if (status === Image.Null)
                                    return 0
                                if ((status===Image.Loading) || (status===Image.Error))
                                    return 55
                                return Math.min( sourceSize.height, mainRect.height - topLine.height - bottomLine.height)
                            }
                        }

                    }

                    Text {
                        id: audioItem
                        width: parent.width - 20
                        anchors.bottomMargin: 3

                        wrapMode: Text.Wrap
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        text: getAudio()
                        color: "lightblue"
                        visible: (picture === "") && (audio !== "")

                        function getAudio()
                        {
                            if(typeof audio === "undefined")
                                return ""
                            else
                                return i18n("Audio: ") + " <a href=\"" + audioUrl + "\">" + audio + "</a>"
                        }

                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }
                    }

                    Text {
                        id: videoItem
                        width: parent.width - 20
                        anchors.bottomMargin: 3

                        wrapMode: Text.Wrap
                        //                        textFormat: Text.StyledText
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        text: getVideo()
                        color: "lightblue"
                        visible: (picture === "") && (video !== "")

                        function getVideo()
                        {
                            if(typeof videoUrl === "undefined")
                                return ""
                            else
                                return i18n("Video: ") + " <a href=\"" + videoUrl + "\">" + video + "</a>"
                        }

                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }
                    }

                    Item {
                        id: msgViewRect
                        x: 10
                        width: parent.width - 20
                        height: msgView.height
                        visible: (picture === "")

                        Flickable {
                            id: msgView
                            width: parent.width
                            height: socialMessage.paintedHeight

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
                                color: "white"
                                clip: true
                                textFormat: Text.StyledText
                                elide: Text.ElideRight
                                maximumLineCount: {
                                    var lines = (mainRect.height - topLine.height - fromItem.height -
                                                 (audioItem.visible ? audioItem.height : 0) -
                                                 (videoItem.visible ? videoItem.height : 0)) / font.pixelSize / 1.5;
                                    return lines
                                }

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
            }

            SocialBottomBar {
                id: bottomLine

                anchors {bottom: parent.bottom; right: parent.right; left: parent.left}
                height: 5

                likesCount : likes
                commentsCount: commentCount
                isLiked: like
                visible: false

                onSendLikeClicked: {
                    if (like) {
                        socialProxy.unlikeItem(id, pluginName);
                    } else {
                        socialProxy.likeItem(id, pluginName);
                    }
                }
                onShowCommentsClicked: {
                    if (cloudRect.state === "details") {
                        //Set source on comments loader
                        commentsEdit.source = "CommentsEditBox.qml"
                        commentsEdit.item.edit.color = "grey"
                        commentsEdit.item.edit.text = i18n("Write comment...")
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

            PropertyChanges { target: bottomLine; height: Math.max( 26, commentsShowText.paintedHeight ); visible: true }

            PropertyChanges { target: modal; enabled: true }

            PropertyChanges {
                target: socialMessage;
                text: message;
                textFormat: Text.RichText;
                visible: true;
                horizontalAlignment: (msgView.contentHeight > msgView.height) ? Text.AlignLeft : Text.AlignHCenter;
            }

            PropertyChanges { target: audioItem; visible: audio !== "" }

            PropertyChanges { target: videoItem; visible: video !== "" }

            PropertyChanges { target: msgMouseArea; enabled: false; z: -1 }

            PropertyChanges { target: msgViewRect; visible: true }

            PropertyChanges { target: msgView; height: msgViewHeight() }

            PropertyChanges { target: msgScrollBar; visible: (msgView.contentHeight > msgView.height) }

            PropertyChanges { target: cloudRect; z: 9000; height: { var y = cloudRect.height; return y } }

            PropertyChanges {
                target: topLine
                state: "detailed"
                dateText: Qt.formatDateTime(datetime, "d MMM " + i18n("in") + " hh:mm")
            }

            PropertyChanges {
                target: socialImage
                width: Math.min(mainRect.width - 20, sourceSize.width)
                height: Math.min( mainRect.height - topLine.height - bottomLine.height - fromItem.height - Math.min(70, msgViewRect.height) - audioItem.height - 20, sourceSize.height)
            }
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

            PropertyChanges { target: bottomLine; commentsShow: true }
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
