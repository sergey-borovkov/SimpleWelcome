import QtQuick 1.1

Item{
    id: galleryRect
    width: 200 //galleryItem.width
    height: galleryItem.height

    property alias commentsListView: commentsListView
    property bool __notDetailed: true
    property int __height: 0

    MouseArea {
        id: modal
        z: -1
        anchors.fill: parent
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
                        text: date
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
                    width: Math.min(200, sourceSize.width)
                    height: Math.min(150, sourceSize.height)
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    smooth: true
                    source: picture
                }
                Text {
                    id: msg
                    width: parent.width
                    anchors.bottomMargin: 3
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: message
                    color: "white"
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
                    width: parent.width
                    anchors.bottomMargin: 3
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: getAudio()
                    color: "lightblue"
                    visible: audio !== ""
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
                    anchors.rightMargin: 30
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
                        id: commensShowAreaText
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                        text: (galleryRect.state === "comments") ? "Hide comments" : "Show comments"
                    }
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        source: "images/arrow.png"
                    }
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: commensShowAreaText.font.bold = true
                        onExited: commensShowAreaText.font.bold = false
                        onClicked: {
                            if (galleryRect.state === "details") {
                                //Set source on comments loader
                                commentsEdit.source = "CommentsEditBox.qml"
                                commentsEdit.item.edit.color = "grey"
                                commentsEdit.item.edit.text = i18n_Write_Comment
                                commentsEdit.item.userPhoto.source = socialProxy.selfPictureUrl()
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
            z: 9999
            onClicked: {
                //console.log("details on")
                if (galleryRect.state === "") {
                    modal.parent = timeFrameTab;
                    modal.z = 300
                    __notDetailed = false
                    __height = galleryItem.childrenRect.height
                    galleryRect.state = "details"
                    socialProxy.getAllComments(id, pluginName)
                }
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

            PropertyChanges { target: bottomLine; height: 26; visible: true }

            PropertyChanges {
                target: galleryRect;
                height: {
                    var y = galleryRect.height
                    return y
                }
            }
            PropertyChanges { target: galleryRect; z: 9000  }            

            PropertyChanges { target: detailsOffArea; visible: true }

            PropertyChanges { target: detailsOnArea; enabled: false }            

            PropertyChanges { target: dateArea; visible: true }

            PropertyChanges { target: likeItem; visible: true }

            PropertyChanges { target: likesCountArea; visible: false }

            PropertyChanges { target: commentsCountArea; visible: false }

            PropertyChanges { target: commentsShowArea; visible: true }

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
