// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: activityItem
//    anchors.fill: parent

    property int typeItem; // 1 - x1, 2 - x2, 3 - x3 - размер прямоугольника
    property string num;

    width: ( typeItem == 1 ) ? x1 : (( typeItem == 2 ) ? x2 : x3 )
    height: ( typeItem == 1 ) ? y1 : (( typeItem == 2 ) ? y2 : y3 )
    anchors.topMargin: ky
    anchors.bottomMargin: ky
    anchors.leftMargin: kx
    anchors.rightMargin: kx

    Rectangle {
        anchors.fill: parent
        border.color: "yellow"
    }


    Component {
        id: imgDelegate
        Item {
            id: imgItem
            anchors.centerIn: parent

            Rectangle {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: ( typeItem == 1 ) ? x1 : (( typeItem == 2 ) ? x2 : x3)
                height: ( typeItem == 1 ) ? y1 : (( typeItem == 2 ) ? y2 : y3)
                color: "blue"
                opacity: cloudDelegate.ListView.isCurrentItem ? 1 : 0.5
            }
            Text {
                text: mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')//  + url
                anchors.centerIn: img
            }
        }
    }

    Component {
        id: videoDelegate
        Item {
            id: videoItem

            Rectangle {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: ( typeItem == 1 ) ? x1 : (( typeItem == 2 ) ? x2 : x3)
                height: ( typeItem == 1 ) ? y1 : (( typeItem == 2 ) ? y2 : y3)
                color: "red"
                opacity: cloudDelegate.ListView.isCurrentItem ? 1 : 0.5
            }
            Text {
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')//  + url
                anchors.centerIn: img
        //        anchors.top: img.bottom
        //        anchors.topMargin: 5
            }
        }
    }

    Component {
        id: docDelegate
        Item {
            id: docItem
            Rectangle {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: ( typeItem == 1 ) ? x1 : (( typeItem == 2 ) ? x2 : x3)
                height: ( typeItem == 1 ) ? y1 : (( typeItem == 2 ) ? y2 : y3)
                color: "green"
                opacity: cloudDelegate.ListView.isCurrentItem ? 1 : 0.5
            }
            Text {
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')//  + url
                anchors.centerIn: img
        //        anchors.top: img.bottom
        //        anchors.topMargin: 5
            }
        }
    }

    function bestDelegate( mt )
    {
        if ( mt === "image" )
        {
            return imgDelegate;
        }
        else if ( mt === "video" )
        {
            return videoDelegate;
        }
        else
        {
            return docDelegate;
        }
    }

    Loader {
        id: eventLoader
        anchors.fill: parent
        sourceComponent: bestDelegate ( mimeType )

    }
}
