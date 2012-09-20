import QtQuick 1.1

Item {
    property int margin: 10
    property int labelHeight: 20

    id: galleryItem
    height: parent.height
    width: getDelegateWidth(size) * (parent.height - 2*margin - dateLabel.height)/3 + 2*margin
    clip: true

    Text {
        id: dateLabel
        anchors.top: parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: (size === 0) ? Qt.formatDate( date , "MMM yyyy") : Qt.formatDate( date , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        //visible: (size === 0) ? false : true
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
    }
*/
    AnimatedImage {
        id: waitIndicator
        source: "images/ajax-loader.gif"
        anchors.centerIn: parent
        visible: (size === 0) ? true : false
    }
    GridView{
        id: itemGrid
        anchors.top: dateLabel.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: localDayModel.itemsModel(date)
        cellWidth: (parent.height - 2*margin - dateLabel.height) / 3 //-20
        cellHeight: (parent.height - 2*margin - dateLabel.height) / 3 -1//-20

        flow: GridView.TopToBottom
        interactive: false
        delegate: Column {
            //Rectangle {
            ItemRectangle{
                id: imageBackground
                //color: "black"
                //border.color: "#687584"
                //border.width: 1
                //radius: 8
                width: itemGrid.cellWidth - 20
                height: itemGrid.cellHeight -40
                //clip: true
                Image {
                    id: image
                    anchors.centerIn: parent
                    width: Math.min( sourceSize.width, parent.width -4)
                    height: Math.min( sourceSize.height, parent.height -4 )
                    fillMode: Image.PreserveAspectFit
                    source: "image://preview/" + url + "%" + Math.random( 10 )
                    smooth: true
                    asynchronous: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally(url)
                }
            }
            Text {
                id: label
                text: url
                color: "white"
                elide: Text.ElideLeft
                width: parent.width
                verticalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

    function getDelegateWidth( count ){
        if (count === 0)
        {
            //galleryLister.startSearch(date,1)   //Start new search if null item becomes visible
            count++
        }
        var x = Math.ceil(count /3)
        return x
    }
}
