import QtQuick 1.1
import Private 0.1

Item{
    id: galleryRect
    width: 200 //galleryItem.width
    height: galleryItem.height

    property alias commentsListView: commentsListView
    property bool __notDetailed: true
    property int __height: 0

    function popupDetailsWidget()
    {
        if (galleryRect.state === "") {
            modal.parent = timeFrameTab;
            modal.z = 300
            __notDetailed = false
            __height = galleryItem.childrenRect.height
            galleryRect.state = "details"
            socialProxy.getAllComments(id, pluginName)
            socialProxy.getAllLikes(id, pluginName)
        }
    }

    function msgViewHeight() {
        var h = mainRect.height - bottomLine.height - topLine.height - 10
        if (img.height)
            h = h - img.height - 10
        if (audioItem.height)
            h = h - audioItem.height - 10;
        if (videoItem.height)
            h = h - videoItem.height - 10;

        return Math.min(msg.height, h)
    }

    MouseArea {
        id: modal
        anchors.fill: parent
        enabled: false
        onClicked:  {
            galleryRect.state = ""
        }
    }

    BorderImage {
        id: innerShadow
        anchors.fill: parent
        border { left: 23; top: 23; right: 23; bottom: 23 }
        source: "images/shadow-inverse.png"
        smooth: true
    }

    Item {
        id: galleryItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 200
        height: (__notDetailed) ? childrenRect.height : __height  //we dont want futher increase height after initial state

        objectName: "SocialGalleryItem"
        property Item mainParent: galleryRect

        ItemRectangle {
            id: mainRect
            width: 200
            height: {
                if (galleryRect.state === "comments")
                    height: 300 + 60 * (commentsCount + 1)
                else
                    Math.max( 120, ( column.height + bottomLine.height ) )
            }
            //anchors.fill: parent
            //clip: true
            Behavior on height {
                NumberAnimation {
                    duration: 150
                }
            }

            Column {
                id: column
                width: parent.width
                anchors.bottomMargin: 10
                anchors.top: parent.top
                spacing: 10

                Item {
                    id: topLine
                    width: parent.width
                    height: Math.max( icon.height, dateArea.paintedHeight )
                    Image {
                        id: icon
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "image://plugin/" + pluginName + "/small"
                    }
                    Text {
                        id: dateArea
                        anchors.centerIn: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: Qt.formatDateTime(datetime, "d MMM yyyy ")
                        color: "white"
                        visible: false
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
                                modal.parent = galleryRect
                                modal.z= -1
                                galleryRect.state = ""
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

                Image { //Main image
                    id: img
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    width: Math.min(200 - 20, sourceSize.width)
                    height: Math.min(150, sourceSize.height)
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    smooth: true
                    source: picture
                }

                Text {
                    function getAudio()
                    {
                        if(typeof audio === "undefined")
                            return ""
                        else
                            return i18n_Audio + " <a href=\"" + audioUrl + "\">" + audio + "</a>"
                    }

                    id: audioItem
                    width: parent.width - 20
                    wrapMode: Text.Wrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: getAudio()
                    color: "lightblue"
                    visible: {
                        var vis = (typeof audio !== "undefined")
                        if (!vis)
                            height = 0;
                        return vis
                    }

                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }

                }

                Text {
                    function getVideo()
                    {
                        if(typeof videoUrl === "undefined")
                            return ""
                        else
                            return i18n_Video + " <a href=\"" + videoUrl + "\">" + video + "</a>"
                    }

                    id: videoItem
                    width: parent.width - 20
                    wrapMode: Text.Wrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: getVideo()
                    color: "lightblue"
                    visible: {
                        var vis = (typeof video !== "undefined")
                        if (!vis)
                            height = 0;
                        return vis
                    }

                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }
                }

                Item {
                    width: parent.width
                    height: msgView.height + 10

                    Flickable {
                        id: msgView
                        width: parent.width
                        height: msg.height
                        contentHeight: msg.paintedHeight
                        clip: true

                        Text {
                            id: msg
                            width: msgView.width - 20
                            anchors.bottomMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            wrapMode: Text.Wrap
                            horizontalAlignment: truncated ? Text.AlignLeft : Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: message

                            color: "white"
                            clip: true
                            textFormat: Text.StyledText
                            elide: Text.ElideRight
                            maximumLineCount: 5

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
            Item {
                id: bottomLine
                width: parent.width
                height: 0
                anchors.bottom: parent.bottom
                anchors.topMargin: 3
                visible: false

                Item {
                    id: bottomCommentsCountArea
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 40
                    height: 22
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
                    id: likeItem
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
                            if (likeItem.state === "") {
                                console.log("add like to item, item id: " + id)
                                socialProxy.likeItem(id, pluginName);
                            } else {
                                console.log("dislike item, item id: " + id)
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
                        text: (galleryRect.state === "comments") ? i18n_Hide_Comments : i18n_Show_Comments
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
                            if (galleryRect.state === "details") {
                                //Set source on comments loader
                                commentsEdit.source = "CommentsEditBox.qml"
                                commentsEdit.item.edit.color = "grey"
                                commentsEdit.item.edit.text = i18n_Write_Comment
                                commentsEdit.item.userPhoto.source = socialProxy.selfPictureUrl(pluginName)
                                commentsListView.view.positionViewAtEnd()
                                galleryRect.state = "comments"
                            }
                            else if (galleryRect.state === "comments")
                            {
                                commentsEdit.source = ""
                                galleryRect.state = "details"
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
                model: repeater.model.comments(index)
            }

            Loader {
                id: commentsEdit
                height: 0
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.rightMargin: 3
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
            //anchors.bottomMargin: 20
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
                target: galleryItem
                anchors.left : undefined
                anchors.right : undefined
                anchors.horizontalCenter : undefined
                anchors.top : undefined
                anchors.bottom : undefined
                anchors.verticalCenter : undefined
            }

            ParentChange {
                target: galleryItem
                parent: timeFrameTab
                width: 400
                height: 300
                x: timeFrameTab.width/2 - galleryItem.width/2
                y: timeFrameTab.height/2 - galleryItem.height/2
            }

            PropertyChanges { target: galleryItem; z: 400}

            PropertyChanges { target: mainRect; width: 400; height: 300 }

            PropertyChanges { target: bottomLine; height: Math.max( 26, commentsShowAreaText.paintedHeight ); visible: true }

            PropertyChanges {
                target: galleryRect;
                height: {
                    var y = galleryRect.height
                    return y
                }
            }
            PropertyChanges { target: galleryRect; z: 9000  }

            PropertyChanges { target: timeFrameTab; enableWheel: false }

            PropertyChanges { target: detailsOffArea; visible: true }

            PropertyChanges { target: dateArea; visible: true; color: "lightgrey"; text: Qt.formatDateTime(datetime, "d MMM " + i18n_In + " hh:mm") }

            PropertyChanges { target: likeItem; visible: true }

            PropertyChanges { target: likesCountArea; visible: false }

            PropertyChanges { target: commentsCountArea; visible: false }

            PropertyChanges { target: commentsShowArea; visible: true }

            PropertyChanges { target: modal; enabled: true }

            PropertyChanges { target: msg; textFormat: Text.RichText }

            PropertyChanges { target: msgMouseArea; enabled: false; z: -1 }

            PropertyChanges { target: msgView; height: msgViewHeight() }

            PropertyChanges { target: msgScrollBar; visible: (msgView.contentHeight > msgView.height) }

            PropertyChanges { target: msg; horizontalAlignment: (msgView.contentHeight > msgView.height) ? Text.AlignLeft : Text.AlignHCenter }


        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: galleryItem; height: 300 + 60 + commentsViewHeight() + 10 }
            PropertyChanges { target: mainRect; height: 300 }
            PropertyChanges { target: commentsRect; height: commentsViewHeight() + 60 + 10}

            PropertyChanges {
                target: commentsListView
                visible: true
                height: commentsViewHeight()
            }

            PropertyChanges {
                target: commentsEdit
                visible: true
                height: 60
            }
        }
    ]

    transitions: [
        Transition {
            ParentAnimation {
                via: foreground
                NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }
            }
        }
    ]

    function commentsViewHeight() {
        if (commentCount < 3)
            return commentCount * 60
        else
            return 180
    }
}
