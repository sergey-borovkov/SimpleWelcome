import QtQuick 1.1

Item {

    property int margin: 10
    property int labelHeight: 20

    id: galleryItem
    height: parent.height
    width: getDelegateWidht(size) * (parent.height - 2*margin - dateLabel.height)/3 + 2*margin
    clip: true

    Text{
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
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
        model: galleryModel.itemsModel(date)
        cellWidth: (parent.height - 2*margin - dateLabel.height) / 3 //-20
        cellHeight: (parent.height - 2*margin - dateLabel.height) /3 //-20
        flow: GridView.TopToBottom
        interactive: false
        delegate: Column {
            Rectangle{
                id: imageBackground
                color: "white"
                border.color: "black"
                border.width: 2
                radius: 5
                width: itemGrid.cellWidth - 4
                height: itemGrid.cellHeight -20
                clip: true
                Image{
                    id: image
                    anchors.centerIn: parent
                    width: Math.min( sourceSize.width, parent.width -4)
                    height: Math.min( sourceSize.height, parent.height -4 )
                    fillMode: Image.PreserveAspectFit
                    source: url
                    smooth: true
                    asynchronous: true
                }
            }


            Text {
                id: label
                text: url
                color: "white"
                elide: Text.ElideLeft
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
