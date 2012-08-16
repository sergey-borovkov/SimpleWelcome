import QtQuick 1.1
Item{
    id: cloudRect
    property date cloudDate
    property variant model
    property int likes: 0
    property int comments: 0
    property string message: ""
    property string picture: ""
    property string pluginName: ""
    property variant pluginIcon : iconPlugin

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



        //visible: index > 3 ? false : true
        //visible: PathView.onPath
        //    BorderImage {
        //        id: innerShadow
        //        anchors.fill: parent
        //        anchors { leftMargin: -6; topMargin: -6; rightMargin: -8; bottomMargin: -8 }
        //        border { left: 5; top: 5; right: 5; bottom: 5 }
        //        //horizontalTileMode: BorderImage.Stretch
        //        //verticalTileMode: BorderImage.Stretch
        //        //anchors.horizontalCenter: parent.horizontalCenter
        //        //anchors.verticalCenter: parent.verticalCenter
        //        source: "images/inner-shadow.png"
        //        smooth: true
        //        visible: false
        //    }

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
                height: 26
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
                anchors.top: parent.top
                anchors.bottom: parent.bottom
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
                    fillMode: Image.PreserveAspectCrop
                    //anchors.leftMargin:
                    //anchors.rightMargin: 1
                    smooth: true
                    source: picture
                    //width: Math.min( sourceSize.width, parent.width-4)
                    //height: Math.min( sourceSize.height, parent.height-4)
                    width: parent.width
                    height: parent.height
                    clip: true
                }
            }


            Rectangle {
                id: bottomLine
                color: "#335f5f5f"
                anchors.bottom: parent.bottom
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

                Text {
                    id: commentsCount
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right:  parent.right
                    anchors.rightMargin: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: "Comments: " + comments
                    color: "grey"
                    visible: false
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
        }


        MouseArea
        {
            id: detailsOnArea
            anchors.fill: parent
            z: 9999
            onClicked: {
                if (cloudRect.state === "")
                {
                    cloudRect.state = "details"
                    modal.parent = timeFrameTab;
                }
                else
                {

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
            }
            AnchorChanges {
                target: bodyItem
                anchors.top: topLine.bottom
                anchors.bottom: bottomLine.top
            }
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
            }
            //PropertyChanges { target: socialImage; width:  }

            PropertyChanges { target: socialMessage; text: message }

            PropertyChanges { target: socialCloudItem; z:1000 }

            PropertyChanges { target: background; color: "white" }

            PropertyChanges { target: socialMessage; color: "black" }

            PropertyChanges { target: likesCount; visible: true }

            PropertyChanges { target: commentsCount; visible: true }

            PropertyChanges { target: likeIcon; visible: false }

            PropertyChanges { target: iconPlugin; visible: false }

            PropertyChanges { target: commentIcon; visible: false }

            PropertyChanges { target: timeScale; z: -1 }

            PropertyChanges { target: topLine; visible: true }

            PropertyChanges { target: detailsOnArea; enabled: false }

            PropertyChanges { target: innerShadow; opacity: 1 }
        }
    ]

    transitions: Transition {
        ParentAnimation {
            ColorAnimation { property: "color"; duration: 300 }
            NumberAnimation { target: socialCloudItem; property: "z" }
            //SequentialAnimation {
                NumberAnimation { properties: "x,y,width,height,opacity,z"; duration: 300 }
              //  NumberAnimation { properties: "z"; duration: 300 }
            //}


            AnchorAnimation { duration: 300 }
        }
    }


    //    Text {
    //        id: img
    //        anchors.horizontalCenter: parent.horizontalCenter
    //        anchors.leftMargin: 5
    //        anchors.rightMargin: 5
    //    }
    //    Image {
    //        id: img
    //        anchors.horizontalCenter: parent.horizontalCenter
    //        fillMode: Image.PreserveAspectFit
    //        anchors.leftMargin: 5
    //        anchors.rightMargin: 5
    //        smooth: true
    //        source: picture
    //    }
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


