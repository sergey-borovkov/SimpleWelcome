
import QtQuick 1.1


Item {
    id: cloudDelegate

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
            num: "1 "
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: cloudItem.bottom
            typeItem: 1
        }
        ActivityItem {
            id: item2
            num: "2 "
            typeItem: 1
            anchors.right: item1.left
            anchors.bottom: cloudItem.bottom

        }
        ActivityItem {
            id: item3
            num: "3 "
            typeItem: 1
            anchors.left: item1.right
            anchors.bottom: cloudItem.bottom

        }
        ActivityItem {
            id: item4
            num: "4 "
            typeItem: 2
            anchors.right: item7.left
            anchors.bottom: item3.top
        }
        ActivityItem {
            id: item5
            num: "5 "
            typeItem: 3
            anchors.left: item6.right
            anchors.bottom: item2.top
        }
        ActivityItem {
            id: item6
            num: "6 "
            typeItem: 3
            anchors.right: item2.left
            anchors.verticalCenter: cloudItem.verticalCenter

        }
        ActivityItem {
            id: item7
            num: "7 "
            typeItem: 3
            anchors.left: item3.right
            anchors.verticalCenter: cloudItem.verticalCenter
        }

        Text {
            id: date
            text: "<b>12 June</b>"
            anchors.right: item6.right
            anchors.bottom: item6.top
        }
    }

}
