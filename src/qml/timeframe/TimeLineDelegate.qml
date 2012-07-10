import QtQuick 1.1

Item {

    property int margin: 10
    property int labelHeight: 20

    id: timeLineItem
    height: parent.height
    width: timeLine.width/3


    /*
    Rectangle{
        id: gridBorder
        border.color: "white"
        color: "transparent"
        border.width: 2
        anchors.top: parent.top
        anchors.left: dateLabel.left
        width: parent.width
        height: parent.height-40
        //visible: (size === 0) ? false : true
    }
    */
    Item {
        id: cloudBorder
        //border.color: "white"
        //color: "transparent"
        y: (index%2 === 1)? parent.height/2 + timeScale.height/2 : 0
        anchors.leftMargin: -50
        //anchors.rightMargin: -25
        width: parent.width + 100
        height: parent.height/2 - timeScale.height/2
        Loader {
            id: cloud
            anchors.fill: parent
            source: getSourceComponent(size)
            onLoaded: {
                   cloud.item.cloudDate = date
                   cloud.item.model = galleryModel.itemsModel(date)
                   cloud.item.createConnection()
            }
        }
    }

    function getSourceComponent(count)
    {
        if (count === 1)
            return "CloudOne.qml"
        else if (count === 2)
            return "CloudTwo.qml"
        else if (count === 3)
            return "CloudThree.qml"
        else if (count === 4)
            return "CloudFour.qml"
        else if (count === 5)
            return "CloudFive.qml"
        else if (count === 6)
            return "CloudSix.qml"
        return "CloudOne.qml"
    }

//        CloudOne{
//            id: cloud
//            anchors.fill: parent
//            date1: date
//            model: galleryModel.itemsModel(date)
//        }
//        Connections{
//            target: galleryModel.itemsModel(date)
//            onGotThumbnail:
//            {
//                console.log("thumbnail")
//                //cloud.url1 = galleryModel.itemsModel(date).url(0)
//                cloud.image1.source = "image://preview/" + galleryModel.itemsModel(date).url(0) + "%" + Math.random( 10 )
//                cloud.image2.source = "image://preview/" + galleryModel.itemsModel(date).url(1) + "%" + Math.random( 10 )
//                console.log(galleryModel.itemsModel(date).url(0) + " " + cloud.date1)
//                console.log(galleryModel.itemsModel(date).url(1) + " " + cloud.date1)
//            }

//        }

//        Text{
//            id: dateLabel
//            anchors.left: parent.left
//            anchors.top : parent.top
//            z: 40
//            width: 100
//            height: labelHeight
//            text: (size === 0) ? Qt.formatDate( date , "MMM yyyy") : Qt.formatDate( date , "dd MMM yyyy")
//            color: "white"
//            horizontalAlignment: Text.AlignHCenter
//            //visible: (size === 0) ? false : true
//        }

//        Flow {
//            id: cloudFlow
//            anchors.fill: parent
//            spacing: 10
//            Repeater {
//                model: galleryModel.itemsModel(date)
//                delegate:  Rectangle {
//                    width: (cloudBorder.width- cloudFlow.spacing*3)/4// -50 + rand()
//                    height: (cloudBorder.height-cloudFlow.spacing)/2 //-50 + rand()
//                    color: "black"
//                    radius: 5
//                    visible: (index < 7)? true : false
//                    Image {
//                        id: image
//                        anchors.centerIn: parent
//                        width: Math.min( sourceSize.width, parent.width -4)
//                        height: Math.min( sourceSize.height, parent.height -4 )
//                        fillMode: Image.PreserveAspectFit
//                        source: "image://preview/" + url + "%" + Math.random( 10 )
//                        smooth: true
//                        asynchronous: true
//                    }
//                }
//            }

//        }
//    }
//    function rand()
//    {

//        var r = Math.random() * 50
//        console.log(r)
//        return r
//    }

    //*/    GridView{
    //        id: itemGrid
    //        anchors.top: dateLabel.bottom
    //        anchors.right: parent.right
    //        anchors.left: parent.left
    //        anchors.bottom: parent.bottom
    //        anchors.margins: 10
    //        model: galleryModel.itemsModel(date)
    //        cellWidth: (parent.height - 2*margin - dateLabel.height) / 3 //-20
    //        cellHeight: (parent.height - 2*margin - dateLabel.height) /3 //-20
    //        flow: GridView.TopToBottom
    //        interactive: false
    //        delegate: Column {
    //            Rectangle{
    //                id: imageBackground
    //                color: "black"
    //                border.color: "black"
    //                border.width: 2
    //                radius: 5
    //                width: itemGrid.cellWidth - 20
    //                height: itemGrid.cellHeight -40
    //                clip: true
    //                Image{
    //                    id: image
    //                    anchors.centerIn: parent
    //                    width: Math.min( sourceSize.width, parent.width -4)
    //                    height: Math.min( sourceSize.height, parent.height -4 )
    //                    fillMode: Image.PreserveAspectFit
    //                    source: "image://preview/" + url + "%" + Math.random( 10 )
    //                    smooth: true
    //                    asynchronous: true
    /*
                        Connections
                        {
                            target: itemGrid.model
                            onDataChanged:
                            {
                                image.source = "image://preview/" + url + "%" + Math.random( 10 )
                            }
                        }
                        */
    //                }
    //                MouseArea{
    //                    anchors.fill: parent
    //                    onClicked: Qt.openUrlExternally(url)
    //                }
    //            }*/


//    Button{
//        id: showContentButton
//        visible: (size === 0) ? true : false
//        anchors.centerIn: parent
//        width: 100
//        height: 60
//        color: "white"
//        ButtonText {
//            id: buttonText
//            text: "show"
//            anchors.left: parent.left
//        }

//        MouseArea{
//            id: buttonMouseArea
//            anchors.fill: parent
//            onClicked: {
//                galleryLister.startSearch(date,1)
//                galleryView.currentIndex = index
//            }
//        }
//    }

//    AnimatedImage {
//        id: waitIndicator
//        source: "images/ajax-loader.gif"
//        anchors.centerIn: parent
//        visible: (size === 0) ? true : false
//    }



//            Text {
//                id: label
//                text: url
//                color: "white"
//                elide: Text.ElideLeft
//                width: parent.width
//                anchors.horizontalCenter: parent.horizontalCenter
//            }

//        }
//    }

//    function getDelegateWidht( count ){
//        if (count === 0)
//        {
//            galleryLister.startSearch(date,1)   //Start new search if null item becomes visible
//            count++
//        }
//        var x = Math.ceil(count /3)
//        return x
//    }
/*
    ListView.onAdd: SequentialAnimation {
        PropertyAction { target: galleryItem; property: "height"; value: 0 }
        NumberAnimation { target: galleryItem; property: "height"; to: parent.height; duration: 250; easing.type: Easing.InOutQuad}
    }
*/
   /*
    ListView.onAdd:
    {
        //galleryView.positionViewAtIndex(index,ListView.Beginning)
    }
    */
        //    {

        //galleryView.incrementCurrentIndex()
    //    console.log("+++" + galleryView.currentIndex)
        //galleryView.positionViewAtEnd();
        //galleryView.positionViewAtIndex(galleryView.currentIndex, ListView.Contain)
  //  }

}
