import QtQuick 1.0

Rectangle {
    id: timeFrameTab
    width: parent.width
    height: 800
    clip: true
    //anchors.top: parent.top
    anchors.topMargin: 16

    function startup() {

    }

    function prevMonth() {
//        if ( cloudItem.focus == true )
            scene.decrementCurrentIndex()
    }

    function nextMonth() {
//        if ( cloudItem.focus == true )
            scene.incrementCurrentIndex()
    }

    Component.onCompleted: startup();
    

    ListModel {
        id: filesModel

        ListElement {
            url: "image.jpg"
            mimeType: "image"
        }
        ListElement {
            url: "video.mp4"
            mimeType: "video"
        }
        ListElement {
            url: "text.doc"
            mimeType: "doc"
        }
        ListElement {
            url: "image.png"
            mimeType: "image"
        }
        ListElement {
            url: "video.avi"
            mimeType: "video"
        }
        ListElement {
            url: "text.odt"
            mimeType: "doc"
        }
    }


    ListView {
        id: scene
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
        model: filesModel//activityModel
        delegate: SceneDelegate {}
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
        orientation: Qt.Horizontal
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
//        preferredHighlightBegin: 0
//        preferredHighlightEnd: 0
//        flickDeceleration: 1000
        highlightMoveDuration: 1000

        Keys.onLeftPressed: {
            console.log( "left key pressed 333..." )
            prevMonth()
        }
    }

    ToolButton {
        id: prevButton
        imageUrl: "images/go-previous.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        onButtonClick: {
            console.log( "left button pressed..." )
            prevMonth()
        }
    }

    ToolButton {
        id: nextButton
        imageUrl: "images/go-next.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onButtonClick: {
            console.log( "right button pressed..." )
            nextMonth()
        }
    }

    Keys.onLeftPressed: {
        console.log( "left key pressed..." )
        prevMonth()
    }

    Keys.onRightPressed: {
        console.log( "right key pressed..." )
        nextMonth()
    }

    Keys.onEscapePressed: {
        Qt.quit()
    }

}
