
import QtQuick 1.1

Item {
    id: cloudDelegate
    /*
    width: 200
    height: 200

    Rectangle{
        anchors.fill: parent
        border.color: "black"
    }



    Image {
        id: pic
        anchors.fill: parent
        source: getItem(0)
        sourceSize.width: 256
        sourceSize.height: 256
    }
*/

    function getItem( idx )
    {
     //   if (index > (activityModel.count()-1))
       //     return ""
        console.log(">>>>>>>>>new item>>>>>>>>>>")
        console.log("activityModel.count " + activityModel.count())
        console.log("activityModel.index " + index)
        //console.log ("date is " + Qt.formatDateTime(currentDate, "dd-MM-yy") );
        var v = activities
        //console.log(v.count)
        console.log("Url is " + v.getUrl(idx))
        console.log("Date is  " + Qt.formatDateTime(v.getDate(idx), "dd-MM-yy"))
        console.log("<<<<<<<<<<<<<<<<<<<<<<<<<<<")

        //console.log ("v is " + activities );
        //console.log ("v count is " + v.count );
        //console.log("activities " + v.getUrl(idx) )
        return v.getUrl(idx);
    }

    property int kx: cloudWidth / 27
    property int ky: cloudHeight / 9
    property int x1: 3*kx
    property int x2: 4*kx
    property int x3: 6*kx
    property int y1: 2*ky
    property int y2: 3*ky
    property int y3: 4*ky

    width: cloudWidth
    height: cloudHeight
    anchors.topMargin: (timeFrameTab.height / 2 - height ) / 2
    anchors.bottomMargin:(timeFrameTab.height / 2 - height ) / 2
    anchors.leftMargin: (timeFrameTab.width / 2 - width ) / 2
    anchors.rightMargin: (timeFrameTab.width / 2 - width ) / 2

    Rectangle {
        anchors.fill: parent
        border.color: "blue"
    }



    Item {
        id: cloudItem
        anchors.fill: parent

        ActivityItem {
            id: item1
            path: getItem(0)
            num: "1 "
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: cloudItem.bottom
            typeItem: 1
        }
        /*
        ActivityItem {
            id: item2
            num: "2 "
            path: getItem(1)
            typeItem: 1
            anchors.right: item1.left
            anchors.bottom: cloudItem.bottom

        }
        ActivityItem {
            id: item3
            num: "3 "
            path: getItem(2)
            typeItem: 1
            anchors.left: item1.right
            anchors.bottom: cloudItem.bottom

        }
        ActivityItem {
            id: item4
            num: "4 "
            path: getItem(3)
            typeItem: 2
            anchors.right: item7.left
            anchors.bottom: item3.top
        }
        ActivityItem {
            id: item5
            num: "5 "
            path: getItem(4)
            typeItem: 3
            anchors.left: item6.right
            anchors.bottom: item2.top
        }
        ActivityItem {
            id: item6
            num: "6 "
            path: getItem(5)
            typeItem: 3
            anchors.right: item2.left
            anchors.verticalCenter: cloudItem.verticalCenter

        }
        ActivityItem {
            id: item7
            num: "7 "
            path: getItem(6)
            typeItem: 3
            anchors.left: item3.right
            anchors.verticalCenter: cloudItem.verticalCenter
        }

        Text {
            id: date
            text: "<b>" + Qt.formatDateTime(currentDate, "dd-MM-yy") + "</b> "
            anchors.right: item6.right
            anchors.bottom: item6.top
        }
        */
    }
}
