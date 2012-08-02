import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width
    clip: true

    property variant grid//: appsGrid

    GridWithGroupTab {
        groups: [
            {
                //group: "Recent Applications",
                dataSource: dataSource_Apps
            }
        ]
    }
//    IconGridView {
//        id: appsGrid
//        dataSource: dataSource_Apps
//        anchors.fill: parent

//        interactive: false
//        focus: true
//        keyNavigationWraps: true
//        //flow: GridView.TopToBottom

//        anchors.top: parent.top
//        anchors.topMargin: 32
//    }

}
