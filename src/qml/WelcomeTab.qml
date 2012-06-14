import QtQuick 1.1

Item {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    //color: "transparent"

    //signal newMatchesFound()

    Component.onCompleted: load();

    function load()
    {
        groupGrid.preload();
    }

    function reload()
    {
        groupGrid.reloadWelcome();
    }

    Flickable {
        id: view
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: groupGrid.height
        boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: rowContainer
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 16
            spacing: 16
            width: parent.width

            GroupGrid {
                id: groupGrid
                gridType: "welcome"
            }
        }

        states: State {
            name: "ShowBars"
            when: view.movingVertically || view.movingHorizontally
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }

    ScrollBar {
        id: verticalScrollBar
        width: 12;
        height: view.height - 12

        anchors.right: view.right
        opacity: 0
        orientation: Qt.Vertical
        position: view.visibleArea.yPosition
        pageSize: view.visibleArea.heightRatio
    }
}
