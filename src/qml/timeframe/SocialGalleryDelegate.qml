import QtQuick 1.1

Item {
    property int margin: 10
    property int labelHeight: 20

    id: socialGalleryItem
    height: parent.height
    width: flow.childrenRect.width + 20

    Text {
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( date , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }
    /*
    Rectangle {
        id: gridBorder
        border.color: "black"
        color: "transparent"
        border.width: 2
        anchors.top: dateLabel.bottom
        anchors.left: dateLabel.left
        width: parent.width + 10
        height: parent.height + 10
        z: -1
    }
    */
    AnimatedImage {
        id: waitIndicator
        source: "images/ajax-loader.gif"
        anchors.centerIn: parent
        visible: (size === 0) ? true : false
    }

    Item {
        id: itemFlow
        anchors.top: dateLabel.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        Flow {
            id: flow

            property real originalHeight: parent.height

            x: 5; y: 5
            height: originalHeight
            spacing: 10
            flow: Flow.TopToBottom

            Repeater {
                id: repeater
                model: socialDayModel.itemsModel(date)
                SocialDelegate { }
            }            
        }
    }


    function getDelegateWidth( count ){        
        if (count === 0)
        {
            galleryLister.startSearch(date,1)   //Start new search if null item becomes visible
            count++
        }
        var x = Math.ceil(count /3)
        return x
    }
    function printRoles()
    {
        console.log("date " + date)
        console.log("size " + size)
        return Math.ceil(size /3)
    }
}
