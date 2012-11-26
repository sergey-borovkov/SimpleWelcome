import QtQuick 1.1
import Private 0.1

Item {
    id: tabRoot
    anchors.fill: parent

    // Dict-defined
    property variant groups
    property bool isForceOnOneScreen: false
    // Other
    property bool isPopupOpened: popupFrame.state === "OPEN"
    property int stackCellOpenedId: -1
    property variant dataSources
    property variant iconPositions

    property alias tabListView: gridsListView
    property alias gridMouseArea: gridsListView.gridMouseArea

    clip: true

    function updateGridsContent() {
        gridsListView.updateGridsContent()
    }

    function processKeyboard(key) {
        if (popupFrame.state === "OPEN")
            popupFrame.gridGroup.gridView.processKeyboard(key)
        else if (gridsListView.currentItem)
            gridsListView.currentItem.processKeyboard(key)
    }

    function closePopup() {
        gridsListView.hideGroup()
    }

    Item {
        id: tabWrapper
        anchors {
            top: parent.top
            topMargin: -popupFrame.slideHeight
        }
        width: parent.width
        height: parent.height + popupFrame.slideHeight

        function draggedOut(item) {
            gridsListView.hideGroup(false)
            //console.log(item.caption + " GOT")

            var gridView = gridsListView.activeGridView
            gridView.newItemData(item)
            gridView.unstackItemInItem(popupFrame.stackedIconIndex, gridView.count - 1)

            gridMouseArea.skipMoveAnimation = true
            // If new page needed
            if (gridView.maxCount !== -1 && gridView.count > gridView.maxCount) {
                gridView.model.move(gridView.count - 2, gridView.count - 1, 1)

                //console.log("Drag OUT and push pop is here")
                gridsListView.gridIconPushPop("Apps", true)
            }
            gridMouseArea.updateCurrentGrid()

            gridMouseArea.startDragging(gridsListView.activeGridView.count - 1)
            popupFrame.stackedIconIndex = -1
        }

        GridWithGroupTabListView {
            id: gridsListView
        }

        // Scroll using mouse wheel
        WheelArea {
            id: wheelArea
            anchors.fill: parent

            onScrollVert: {
                if (popupFrame.state === "CLOSED") {
                    if (gridMouseArea.pressed && gridMouseArea.dndSrcId != -1) {
                        gridMouseArea.dragIconToPrevNextTab(delta <= 0)
                    }
                    else {
                        if (delta > 0)
                            gridsListView.decrementCurrentIndex()
                        else
                            gridsListView.incrementCurrentIndex()
                    }
                }
            }
        }

        // Bottom dots for displaying current tab index
        Row {
            id: bottomDots
            anchors {
                top: gridsListView.bottom;
                horizontalCenter: gridsListView.horizontalCenter
            }
            width: childrenRect.width
            height: 30

            spacing: 20
            visible: gridsListView.count > 1

            Repeater {
                model: gridsListView.count

                Rectangle {
                    id: dot
                    width: 6
                    height: 6

                    radius: 3
                    color: gridsListView.currentIndex == index ? "white" : "gray"

                    MouseArea {
                        id: mouse
                        width: 26
                        height: 30
                        anchors.centerIn: parent
                        onClicked: gridsListView.currentIndex = index
                    }

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
            }
        }

        // Background darkener for displaying popup grid with stacked icons
        Rectangle {
            anchors {
                top: parent.top
                topMargin: -15
            }
            width: parent.width
            height: parent.height + 28

            color: Qt.rgba(0, 0, 0, 0.6)
            opacity: popupFrame.state == "OPEN"

            Behavior on opacity {
                NumberAnimation { duration: 200; /*easing.type: Easing.OutQuint*/ }
            }
        }

        // Popup grid with stacked icons
        PopupFrame {
            id: popupFrame
            width: parent.width
            z: 1

            property int slideHeight: 0
            property int stackedIconIndex: -1

            state: "CLOSED"

            function groupCloseCompleted() {
                if (gridMouseArea.dndSrcId === -1)
                    gridsListView.dndStateChanged(false)
            }

            states: [
                State {
                    name: "CLOSED"
                    PropertyChanges {
                        target: popupFrame
                        height: 0
                        slideHeight: 0
                    }
                },

                State {
                    name: "OPEN"
                    PropertyChanges {
                        target: popupFrame
                        height: childrenRect.height
                        slideHeight: y + height < tabRoot.height ? 0 : -(tabRoot.height - (y + height - 6))
                    }
                }
            ]

            transitions: [
                Transition {
                    from: "CLOSED"
                    to: "OPEN"

                    NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
                },
                Transition {
                    from: "OPEN"
                    to: "CLOSED"

                    SequentialAnimation {
                        NumberAnimation { properties: "height, slideHeight"; easing.type: Easing.InOutQuad }
                        ScriptAction { script: popupFrame.groupCloseCompleted() }
                    }
                }
            ]

            Component.onCompleted: {
                gridGroup.groupNameChanged.connect(gridsListView.groupNameChanged)
            }
        } // popupFrame
    } // tabWrapper
} // tabRoot
