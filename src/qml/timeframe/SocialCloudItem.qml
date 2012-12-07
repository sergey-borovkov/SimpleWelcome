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

    property int viewWidth: 450
    property int viewHeight: 300

    Connections {
        // while specifying socialDayModel here is ugly any other way generates warnings
        // either about non-existent properties or in case of dynamic connection about
        // unknown property index
        target: socialDayModel.itemsModel(date)
        onUpdateData: {
            if(index === -1) {
                return;
            }

            likes = model.likesCount(index)
            commentCount = model.commentsCount(index)
            like = model.like(index)
        }
    }

    function initialize(idx)
    {
        index = idx
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

    function msgViewHeight() {
//        var h = msgView.parent.height - 10
        var h = bodyItem.height - 20
        h -= socialImage.height * socialImage.visible
        h -= audioItem.height * audioItem.visible
        h -= videoItem.height * videoItem.visible

        return Math.min(msgView.socialMessage.height, h)
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

    function textVisible()
    {
        if (picture === "")
        {
            msgView.socialMessage.text = ""
            return false
        }
        return true
    }

    function commentsViewHeight() {
        return commentCount < 3 ? commentCount * 60 : 180
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
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        width: parent.width
        height: parent.height

        objectName: "SocialCloudItem"
        property Item mainParent: cloudRect

        ItemRectangle {
            id: mainRect
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width
            height: parent.height

            SocialTopBar {
                id: topLine
                anchors.top: parent.top
                width: parent.width
                height: 26

                onExitClicked: {
                    modal.parent = cloudRect
                    modal.z = -1
                    cloudRect.state = ""
                }

                dateText : date
                likesCount : likes
                commentsCount: commentCount
            }

            FromItem {
                id: fromItem
                userName: fromName
                userImage: fromImageUrl
                width: parent.width - 20
                anchors { top: topLine.bottom; bottomMargin: 10 }
            }

            Item {
                id: bodyItem
                anchors {
                    top: fromItem.bottom
                    bottom: bottomLine.top
                    left: parent.left
                    right: parent.right
                }

                Column {
                    id: column
                    anchors.fill: parent
                    spacing: 10

                    Item {
                        id: imageAnchor
                        width: parent.width
                        height: parent.height
                        visible: picture !== ""

                        SocialImage { //Main image
                            id: socialImage
                            anchors {
                                fill: parent
                                horizontalCenter: parent.horizontalCenter
                                verticalCenter: parent.verticalCenter
                                leftMargin: 5
                                rightMargin: 5
                            }

                            width: getWidth()
                            height: getHeight()

                            function getWidth() {
                                if (status === Image.Null)
                                    return 0
                                if (status === Image.Loading || status === Image.Error)
                                    return 55
                                return Math.min(sourceSize.width, parent.width - 20)
                            }

                            function getHeight() {
                                if (status === Image.Null)
                                    return 0
                                if (status === Image.Loading || status === Image.Error)
                                    return 55

                                return Math.min(sourceSize.height, parent.height)
                            }

                            source: picture
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                        }
                    }

                    Text {
                        id: audioItem
                        width: parent.width - 20
                        anchors {
                            bottomMargin: 3
                            horizontalCenter: parent.horizontalCenter
                        }

                        function getAudio()
                        {
                            if(typeof audio === "undefined")
                                return ""
                            else
                                return i18n("Audio: ") + " <a href=\""
                                        + audioUrl + "\"><font color=\"#84c0ea\">"
                                        + audio + "</font></a>"
                        }

                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        text: getAudio()
                        color: "lightblue"
                        visible: (picture === "") && (audio !== "")
                    }

                    Text {
                        id: videoItem
                        width: parent.width - 20
                        anchors {
                            bottomMargin: 3
                            horizontalCenter: parent.horizontalCenter
                        }

                        function getVideo()
                        {
                            if(typeof videoUrl === "undefined")
                                return ""
                            else
                                return i18n("Video: ") + " <a href=\""
                                        + videoUrl + "\"><font color=\"#84c0ea\">"
                                        + video + "</font></a>"
                        }

                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        text: getVideo()
                        color: "lightblue"
                        visible: (picture === "") && (video !== "")
                    }

                    SocialMessageView {
                        id: msgView
                        width: parent.width - 20
                        height: parent.height
                        x: 10

                        visible: picture === ""
                    }
                }

                MouseArea {
                    id: msgMouseArea
                    anchors.fill: parent
                    onClicked: {
                        popupDetailsWidget()
                    }

                    hoverEnabled: true
                }
            }

            SocialBottomBar {
                id: bottomLine

                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left
                }
                height: 5

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
                    else if (cloudRect.state === "comments") {
                        commentsEdit.source = ""
                        cloudRect.state = "details"
                    }
                }

                likesCount : likes
                commentsCount: commentCount
                isLiked: like
                visible: false
            }
        }

        ItemRectangle {
            id: commentsRect
            anchors {
                top: mainRect.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                leftMargin: 30
                topMargin: -10
                rightMargin: 30
                bottomMargin: 8
            }
            height: 0
            z: -1

            CommentsListView {
                id: commentsListView
                anchors {
                    top : parent.top
                    left: parent.left
                    right: parent.right
                    bottom: commentsEdit.top
                    topMargin: 15
                    leftMargin: 3
                    rightMargin: 3
                    bottomMargin: 5
                }
                visible: false
            }

            Loader {
                id: commentsEdit
                height: 0
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    leftMargin: 1
                    rightMargin: 1
                }

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
                x: timeFrameTab.width / 2 - socialCloudItem.width / 2
                y: timeFrameTab.height / 2 - socialCloudItem.height / 2
                width: viewWidth
                height: viewHeight
            }

            PropertyChanges { target: socialCloudItem; z: 400 }

            PropertyChanges { target: timeFrameTab; enableWheel: false }

            PropertyChanges { target: bottomLine; height: Math.max( 26, commentsShowText.paintedHeight ); visible: true }

            PropertyChanges { target: modal; enabled: true }

            PropertyChanges {
                target: msgView.socialMessage
                text: message
                textFormat: Text.RichText
                horizontalAlignment: (msgView.contentHeight > msgView.height) ? Text.AlignLeft : Text.AlignHCenter
            }

            PropertyChanges { target: audioItem; visible: audio !== "" }

            PropertyChanges { target: videoItem; visible: video !== "" }

            PropertyChanges { target: msgMouseArea; enabled: false; z: -1 }

            PropertyChanges {
                target: msgView
                height: msgViewHeight()
                visible: true
            }

            PropertyChanges { target: msgView.scrollBar; visible: (msgView.view.contentHeight > msgView.view.height) }

            PropertyChanges { target: cloudRect; z: 9000; height: { var y = cloudRect.height; return y } }

            PropertyChanges {
                target: topLine
                state: "detailed"
                dateText: Qt.formatDateTime(datetime, "d MMM " + i18n("in") + " hh:mm")
            }

            PropertyChanges {
                target: imageAnchor
                height: {
                    var h = parent.height
                    if(msgView.socialMessage.text !== "" || audioItem.visible
                            || videoItem.visible) {
                        h *= 2 / 3
                    }
                    return h
                }
            }
        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: socialCloudItem; height: viewHeight + commentsViewHeight() + 60 + 10}
            PropertyChanges { target: mainRect; height: viewHeight }
            PropertyChanges { target: commentsRect; height: commentsViewHeight() + 60 + 10}

            PropertyChanges {
                target: commentsListView
                visible: true
                height: commentsViewHeight()
            }

            PropertyChanges {
                target: commentsEdit
                height: 60
                visible: true
            }

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

}
