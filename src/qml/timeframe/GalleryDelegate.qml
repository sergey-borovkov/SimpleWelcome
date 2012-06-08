import QtQuick 1.1

Item {
    id: galleryItem
    height: parent.height
    width: getDelegateWidht(size) * parent.height/3
    clip: true

    Text{
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: 20
        text: Qt.formatDate( date , "dd-MM-yyyy")
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
        model: galleryModel.itemsModel(ListView.currentIndex)
        cellWidth: parent.height /3 -20
        cellHeight: itemGrid.height /3 -20
        flow: GridView.TopToBottom
        interactive: false
        delegate: Column {
            Image{
                id: image
                anchors.horizontalCenter: parent.horizontalCenter
                width: itemGrid.cellWidth -3
                height: itemGrid.cellHeight -20
                source: url
                smooth: true
                asynchronous: true
            }
            Text {
                id: label
                text: url
                color: "white"
                elide: Text.ElideRight
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    function getDelegateWidht( count ){
        var x = Math.ceil(count /3)
        return x
    }
}
