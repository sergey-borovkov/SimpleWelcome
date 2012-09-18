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
        anchors.fill: parent
    }

    BorderImage {
        id: innerShadow
        anchors.fill: parent
        border { left: 23; top: 23; right: 23; bottom: 23 }
        source: "images/inner-shadow.png"
        smooth: true
        opacity: 0
    }


    Item {
        id: galleryItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 200
        height: (__notDetailed) ? childrenRect.height : __height  //we dont want futher increase height after initial state

        ShadowRectangle {
            id: shadowRect
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
                anchors.topMargin: 2
                anchors.bottomMargin: 10
                anchors.top: parent.top
                spacing: 10

                Item {
                    width: parent.width
                    height: Math.max( icon.height, dt.paintedHeight )
                    Image {
                        id: icon
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        width: 32
                        height: 32
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "image://plugin/" + pluginName
                    }
                    Text {
                        id: dt
                        anchors.left: icon.right
                        //anchors.right: parent.right
                        anchors.leftMargin: 3
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: date
                        color: "grey"
                    }
                    Item {
                        id: detailsOffArea
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        width: 20
                        height: 20
                        visible: false
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
                                modal.parent = galleryRect
                                galleryRect.state = ""
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
                height: 26//Math.max( commentsCountText.paintedHeight, dt.paintedHeight )
                anchors.bottom: commentsListView.top
                anchors.topMargin: 3

                Item {
                    id: commentsCount
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    width: 100
                    height: parent.height
                    //                    visible: false
                    Text {
                        anchors.fill: parent
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: i18n_Comments + commentCount
                        color: "grey"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if (galleryRect.state === "details") {
                                //Set source on comments loader
                                commentsEdit.source = "CommentsEditBox.qml"
                                commentsEdit.item.edit.color = "grey"
                                commentsEdit.item.edit.text = i18n_Write_Comment
                                galleryRect.state = "comments"
                            }
                            else if (galleryRect.state === "comments") {
                                //Destroy loader component
                                commentsEdit.source = ""
                                galleryRect.state = "details"
                            }
                        }
                    }
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
                        text: i18n_Like
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
                            PropertyChanges { target: likesText; text: i18n_Dislike }
                            //PropertyChanges { target: likeSendArea; enabled: false }
                        }
                    ]
                }
                Item {
                    id: likeCountItem
                    anchors.right: parent.right
                    anchors.rightMargin: 3
                    height: parent.height
                    width: 30
                    Image {
                        id: likeIcon
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 16
                        height: 16
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/thumb-up.png"
                    }
                    Text {
                        id: likesCountText
                        anchors.left: likeIcon.right
                        anchors.leftMargin: 3
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: likes
                        color: "grey"
                    }
                }
            }
            ListView {
                id: commentsListView
                //anchors.top : bottomLine.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: commentsEdit.top
                clip: true
                height: 0
                visible: false
                model: repeater.model.comments(index)
                delegate: Item {
                    width: 200; height: 60
                    Image  {
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

                    ListView.onAdd: {
                        console.log("123")
                        commentsListView.positionViewAtEnd()
                    }
                }
            }/*
            Connections {
                target: repeater.model
                onUpdateData: {
                    console.log("update!!!!")
                    commentsListView.model = repeater.model.comments(index)
                }
            }
*/
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
                    commentsListView.positionViewAtEnd()
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
                    __notDetailed = false
                    __height = galleryItem.childrenRect.height
                    galleryRect.state = "details"
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
            PropertyChanges { target: shadowRect; width: 400; height: 300 }
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

            PropertyChanges { target: innerShadow; opacity: 1 }

            PropertyChanges { target: likeItem; visible: true }
        },
        State {
            name: "comments" ; extend: "details"

            PropertyChanges { target: galleryItem; height: 300 + 60 * ( commentsListView.model.rowCount() + 1) }

            PropertyChanges { target: shadowRect; height: galleryItem.height }

            PropertyChanges {
                target: commentsListView
                visible: true
                height: 60 * commentsListView.model.rowCount()
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
}
