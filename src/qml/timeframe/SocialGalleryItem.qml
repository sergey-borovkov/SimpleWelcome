import QtQuick 1.1
import Private 0.1

Item{
    id: galleryRect
    width: 230 //galleryItem.width
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
        var h = mainRect.height - bottomLine.height - topLine.height - fromItem.height - 10
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
        width: galleryRect.width
        height: (__notDetailed) ? childrenRect.height : __height  //we dont want futher increase height after initial state

        objectName: "SocialGalleryItem"
        property Item mainParent: galleryRect

        ItemRectangle {
            id: mainRect
            width: parent.width
            height: {
                if (galleryRect.state === "comments")
                    height: 300 + 60 * (commentsCount + 1)
                else
                    Math.max( 120, ( column.height + bottomLine.height + topLine.height + fromItem.height) )
            }
            //anchors.fill: parent
            //clip: true
            Behavior on height {
                NumberAnimation {
                    duration: 150
                }
            }

            SocialTopBar {
                id: topLine

                anchors.top: parent.top
                width: parent.width
                height: 26

                dateText : date
                likesCount : likes
                commentsCount: commentCount
                pluginIcon: "image://plugin/" + pluginName + "/small"


                onExitClicked: {
                    modal.parent = galleryRect
                    modal.z = -1
                    galleryRect.state = ""
                }
            }

            FromItem {
                id: fromItem
                userName: fromName
                userImage: fromImageUrl
                width: parent.width - 20
                anchors { top: topLine.bottom; bottomMargin: 10 }
            }

            Column {
                id: column

                anchors { top: fromItem.bottom; bottomMargin: 10 }
                width: parent.width

                spacing: 10

                SocialImage { //Main image
                    id: img
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    width: getWidth()
                    height: getHeight()
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    smooth: true
                    source: picture

                    function getWidth() {
                        if (status === Image.Null)
                            return 0
                        if ((status===Image.Loading) || (status===Image.Error))
                            return 55
                        return Math.min(200 - 20, sourceSize.width)
                    }
                    function getHeight() {
                        if (status === Image.Null)
                            return 0
                        if ((status===Image.Loading) || (status===Image.Error))
                            return 55
                        return Math.min(150, sourceSize.height)
                    }
                }

                Text {
                    function getAudio()
                    {
                        if(typeof audio === "undefined")
                            return ""
                        else
                            return i18n("Audio: ") + " <a href=\"" + audioUrl + "\"><font color=\"#84c0ea\">" + audio + "</font></a>"
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

                Item {
                    id: videoItem

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: childrenRect.width
                    height: childrenRect.height

                    Column {

                        Image {
                            id: videoPreview

                            anchors.horizontalCenter: parent.horizontalCenter
                            width: Math.min(200 - 20, sourceSize.width)
                            height: Math.min(150, sourceSize.height)

                            source: (videoImage === undefined)? "" : videoImage
                            fillMode: Image.PreserveAspectFit

                            Image { //Overlay play icon
                                id: playIcon

                                anchors.centerIn: parent
                                source: "images/play-empty.png"
                                visible: videoPreview.status === Image.Ready
                            }
                            MouseArea{
                                id: videoPreviewMArea
                                anchors.fill: parent
                                enabled: false
                                onClicked: Qt.openUrlExternally(videoUrl)
                            }
                        }
                        Text {

                            id: videoCaption
                            width: galleryItem.width - 20
                            anchors.horizontalCenter: parent.horizontalCenter

                            function getVideo()
                            {
                                if(typeof videoUrl === "undefined")
                                    return ""
                                else
                                    return " <a href=\""
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
                            visible: false //caption visible only in "detailed" state

                        }
                    }
                }

                Item {
                    x: 10
                    width: parent.width - 20
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

            SocialBottomBar {
                id: bottomLine

                anchors {bottom: parent.bottom; topMargin: 3; right: parent.right; left: parent.left}
                height: 0

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
                    if (galleryRect.state === "details") {
                        //Set source on comments loader
                        commentsEdit.source = "CommentsEditBox.qml"
                        commentsEdit.item.edit.color = "grey"
                        commentsEdit.item.edit.text = i18n("Write comment...")
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
                onSend: socialProxy.commentItem(comment, id, pluginName)
            }
        }
        MouseArea
        {
            id: detailsOnArea
            anchors.fill: parent            
            z: -2
            onClicked: popupDetailsWidget()
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

            PropertyChanges { target: bottomLine; height: Math.max( 26, commentsShowText.paintedHeight ); visible: true }

            PropertyChanges {
                target: galleryRect;
                height: {
                    var y = galleryRect.height
                    return y
                }
            }
            PropertyChanges { target: galleryRect; z: 9000  }

            PropertyChanges { target: timeFrameTab; enableWheel: false }            

            PropertyChanges { target: topLine; state: "detailed"; dateText: Qt.formatDateTime(datetime, "d MMM " + i18n("in") + " hh:mm") }

            PropertyChanges { target: modal; enabled: true }

            PropertyChanges { target: msg; textFormat: Text.RichText }

            PropertyChanges { target: msgMouseArea; enabled: false; z: -1 }

            PropertyChanges { target: msgView; height: msgViewHeight() }

            PropertyChanges { target: msgScrollBar; visible: (msgView.contentHeight > msgView.height) }

            PropertyChanges { target: msg; horizontalAlignment: (msgView.contentHeight > msgView.height) ? Text.AlignLeft : Text.AlignHCenter }

            PropertyChanges { target: videoCaption; visible: true }

            PropertyChanges { target: videoPreviewMArea; enabled: true }

            PropertyChanges {
                target: videoPreview
                width: Math.min(galleryItem.width - 20, sourceSize.width)
                height: {
                    var h = galleryItem.height - topBar.height - bottomBar.height - fromItem.height
                    if (message === "")
                        return Math.min(sourceSize.height, h - videoCaption.height)
                    else
                        return Math.min(sourceSize.height, 150)
                }
            }

        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: galleryItem; height: 300 + 60 + commentsViewHeight() + 10 }
            PropertyChanges { target: mainRect; height: 300 }
            PropertyChanges { target: commentsRect; height: commentsViewHeight() + 60 + 10}
            PropertyChanges { target: bottomLine; commentsShow: true }

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
