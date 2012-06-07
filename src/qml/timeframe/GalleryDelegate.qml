import QtQuick 1.1

Item {
    id: galleryItem
    height: parent.height
    width: getDelegateWidht(items.count) * parent.height/3
    clip: true

    Text{
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: 20
        text: date
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }
    Rectangle{
        id: gridBorder
        border.color: "black"
        color: "transparent"
        border.width: 2
        anchors.top: dateLabel.bottom
        anchors.left: dateLabel.left
        width: parent.width + 10
        height: parent.height + 10
    }

    GridView{
        id: itemGrid
        anchors.top: dateLabel.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: items
        cellWidth: parent.height /3 -20
        cellHeight: itemGrid.height /3 -20
        flow: GridView.TopToBottom
        interactive: false
        delegate: Column {
            Rectangle{
                id: back
                color: "white"
                radius: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: itemGrid.cellWidth -3
                height: itemGrid.cellHeight -20
            }
            Text {
                id: label
                text: description
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }



    function getDelegateWidht( count ){
        var x = Math.ceil(count /3)
        return x
    }
}
