import QtQuick 1.1

FocusScope {
    id: welcomeTab
    width: parent.width
    clip: true
    anchors.topMargin: 16

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: rowContainer2.height + 32
        boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: rowContainer2
            //anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: childrenRect.height
            anchors.bottom: parent.bottom
            spacing: 32

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height

                Text {
                    width: parent.width
                    height: 24
                    maximumLineCount: 1
                    anchors.left: parent.left
                    anchors.leftMargin: 16

                    font.family: "Bitstream Vera Sans"
                    font.bold: true
                    font.pixelSize: 18
                    style: Text.Sunken
                    color: "#eee"
                    styleColor: "#000"

                    text: "Recent Applications"
                }

                AppsGridView {
                    id: recentAppsGridView
                    model: recentAppsGridModel
                    width: parent.width
                    height: childrenRect.height

                    interactive: false
                    focus: true

                    Keys.onPressed: {
                        switch (event.key)
                        {
                        case Qt.Key_Left:
                            moveCurrentIndexLeft()
                            event.accepted = true
                            break
                        case Qt.Key_Right:
                            if (currentIndex == count - 1)
                                favoritesGridView.focus = true

                            moveCurrentIndexRight()
                            event.accepted = true
                            break
                        case Qt.Key_Up:
                            moveCurrentIndexUp()
                            event.accepted = true
                            break
                        case Qt.Key_Down:
                            if (currentIndex >= count - columns)
                                favoritesGridView.focus = true

                            moveCurrentIndexDown()
                            event.accepted = true
                            break
                        }
                    }
                }
            }

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height

                Text {
                    width: parent.width
                    height: 24
                    maximumLineCount: 1
                    anchors.left: parent.left
                    anchors.leftMargin: 16

                    font.family: "Bitstream Vera Sans"
                    font.bold: true
                    font.pixelSize: 18
                    style: Text.Sunken
                    color: "#eee"
                    styleColor: "#000"

                    text: "Favorites"
                }

                AppsGridView {
                    id: favoritesGridView
                    model: favoritesGridModel
                    width: parent.width
                    height: childrenRect.height
                    interactive: false

                    Keys.onPressed: {
                        switch (event.key)
                        {
                        case Qt.Key_Left:
                            moveCurrentIndexLeft()
                            event.accepted = true
                            break
                        case Qt.Key_Right:
                            moveCurrentIndexRight()
                            event.accepted = true
                            break
                        case Qt.Key_Up:
                            moveCurrentIndexUp()
                            event.accepted = true
                            break
                        case Qt.Key_Down:
                            moveCurrentIndexDown()
                            event.accepted = true
                            break
                        }
                    }

                }
            }

            Column {
                spacing: 16
                width: parent.width
                height: childrenRect.height

                Text {
                    width: parent.width
                    height: 24
                    maximumLineCount: 1
                    anchors.left: parent.left
                    anchors.leftMargin: 16

                    font.family: "Bitstream Vera Sans"
                    font.bold: true
                    font.pixelSize: 18
                    style: Text.Sunken
                    color: "#eee"
                    styleColor: "#000"

                    text: "Recent documents"
                }

                AppsGridView {
                    model: documentsGridModel
                    width: parent.width
                    height: childrenRect.height
                    interactive: false
                }
            }
        }

        states: State {
            name: "ShowBars"
            when: flick.movingVertically
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }

    ScrollBar {
        id: verticalScrollBar
        width: 12;
        height: flick.height - 12

        anchors.right: flick.right
        opacity: 0
        orientation: Qt.Vertical
        position: flick.visibleArea.yPosition
        pageSize: flick.visibleArea.heightRatio
    }
}
