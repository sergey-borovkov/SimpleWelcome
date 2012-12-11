import QtQuick 1.1
import Effects 1.0

Item {
    id: cell

    property int cellWidth: constants.cellWidth

    width: cellWidth
    height: wrapper.height

    GridView.onRemove: {
        wrapper.state = "REMOVING"
    }

    Item {
        id: wrapper
        parent: grid
        x: cell.x
        y: cell.y
        width: cellWidth
        height: innerWrapper.height

        Item {
            id: innerWrapper
            anchors {
                left: parent.left
                top: parent.top
            }

            width: cellWidth
            height: childrenRect.height

            BorderImage {
                id: stackedIconBg
                anchors{
                    left: cellIcon.left
                    top: cellIcon.top
                }
                border {
                    left: 11
                    top: 11
                    right: 16
                    bottom: 15
                }
                width: cellIcon.width + 5
                height: cellIcon.height + 4

                source: "image://generalicon/asset/stacked_icon_bg.png"
                visible: {
                    if (stack !== undefined)
                        return true
                    return false
                }
            }

            Item {
                id: cellIcon
                anchors {
                    top: parent.top
                    topMargin: constants.iconSize / 5 //20
                    horizontalCenter: parent.horizontalCenter
                }
                width: constants.iconSize
                height: constants.iconSize

                Image {
                    id: cellIconImage
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }

                    source: imagePath
                    cache: false

                    /*effect: DropShadow {
                        blurRadius: 50
                        color: Qt.rgba(0, 0, 0, 0.3)
                        xOffset: 0.5
                        yOffset: 0.5
                    }*/

                    Connections {
                        target: mainWindow
                        onIconSizeChanged: {
                            cellIconImage.sourceSize.width = constants.iconSize
                            cellIconImage.sourceSize.height = constants.iconSize
                        }
                    }
                }
            }

            Text {
                id: cellText
                anchors {
                    top: cellIcon.bottom
                    topMargin: constants.iconSize / 8
                    horizontalCenter: parent.horizontalCenter
                }
                width: parent.width - 10

                text: caption

                /*effect: DropShadow {
                    blurRadius: 3
                    color: Qt.rgba(0, 0, 0, 1)
                    xOffset: 0.3
                    yOffset: 0.7
                }*/
                style: Text.Raised
                styleColor: "#000"
                color: "#eee"
                font {
                    bold: true
                    family: "Bitstream Vera Sans"
                    pointSize: constants.iconTextSize
                }

                maximumLineCount: 3
                elide: Text.ElideRight
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
            }

            // Necessary for correct highlight height
            Item {
                height: cellIcon.anchors.topMargin + cellIcon.height + cellText.anchors.topMargin + cellText.height + cellText.anchors.topMargin
            }

        } // innerWrapper

        Image {
            id: pinIcon
            x: cellIcon.x + cellIcon.width/2 + constants.iconSize / 2
            y: cellIcon.y - 25
            z: 1

            opacity: pinned !== undefined && pinned || gridMouseArea.grid === grid && gridMouseArea.hoveredId === id ? 1 : 0
            source: "image://generalicon/asset/clip_icon_hover.png"

            Behavior on opacity {
                NumberAnimation { duration: 300 }
            }

            states: [
                State {
                    name: "iconHover"
                    when: gridMouseArea.grid === grid && gridMouseArea.hoveredId === id && !gridMouseArea.pinHovered && pinned !== true
                    PropertyChanges { target: pinIcon; source: "image://generalicon/asset/clip_icon_hover.png" }
                },

                State {
                    name: "buttonHover"
                    when: gridMouseArea.grid === grid && gridMouseArea.hoveredId === id && gridMouseArea.pinHovered && !gridMouseArea.pressed && pinned !== true
                    PropertyChanges { target: pinIcon; source: "image://generalicon/asset/clip_button_hover.png" }
                },

                State {
                    name: "clipPressed"
                    when: gridMouseArea.grid === grid && gridMouseArea.hoveredId === id && gridMouseArea.pinHovered && gridMouseArea.pressed
                    PropertyChanges { target: pinIcon; source: "image://generalicon/asset/clip_pressed.png" }
                },

                State {
                    name: "clipActiveHover"
                    when: gridMouseArea.grid === grid && gridMouseArea.hoveredId === id && gridMouseArea.pinHovered && pinned !== undefined && pinned
                    PropertyChanges { target: pinIcon; source: "image://generalicon/asset/clip_active_hover.png" }
                },

                State {
                    name: "clipActive"
                    when: pinned !== undefined && pinned
                    PropertyChanges { target: pinIcon; source: "image://generalicon/asset/clip_active.png" }
                }
            ]
        }


        Behavior on x { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }
        Behavior on y { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }

        states: [
            State {
                name: "cellOpenedStack"
                when: stackCellOpenedId === id && stack !== undefined

                ParentChange {
                    target: wrapper
                    parent: tabWrapper
                }
            },

            State {
                name: "cellWasInDrag"
                when: gridMouseArea.dndSrcId === id && gridMouseArea.grid !== grid
            },

            State {
                name: "gridInDrag"
                when: gridMouseArea.dndSrcId != -1 && gridMouseArea.dndSrcId != id && state !== "REMOVING"
            },

            State {
                name: "cellInDrag"
                when: gridMouseArea.dndSrcId === id && gridMouseArea.grid === grid && !gridMouseArea.skipMoveAnimation

                ParentChange {
                    target: wrapper
                    parent: tabRoot
                    x: gridMouseArea.mapToItem(tabRoot, gridMouseArea.mouseX - wrapper.width/2, 0).x
                    y: gridMouseArea.mapToItem(tabRoot, 0, gridMouseArea.mouseY - wrapper.height/2).y
                }
            },

            State {
                name: "cellInDragSkipMoveAnimation"
                when: gridMouseArea.dndSrcId === id && gridMouseArea.grid === grid && gridMouseArea.skipMoveAnimation

                ParentChange {
                    target: wrapper
                    parent: tabRoot
                    x: gridMouseArea.mapToItem(tabRoot, gridMouseArea.mouseX - wrapper.width/2, 0).x
                    y: gridMouseArea.mapToItem(tabRoot, 0, gridMouseArea.mouseY - wrapper.height/2).y
                }
            },

            State {
                name: "REMOVING"

                ParentChange {
                    target: wrapper
                    parent: tabRoot
                }
            }

        ]

        transitions: [
            Transition {
                from: ""
                to: "cellInDrag"
                reversible: true
                ParentAnimation {
                    via: tabRoot
                    NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad; duration: 100 }
                }
            },
            Transition {
                from: "cellOpenedStack"
                ParentAnimation {
                    via: tabWrapper
                    NumberAnimation { duration: 200 }
                }
            },
            Transition {
                from: "cellInDrag"
                to: "REMOVING"

                SequentialAnimation {
                    PropertyAction { target: cell; property: "GridView.delayRemove"; value: true }
                    NumberAnimation { target: wrapper; property: "scale"; to: 0; duration: 250; easing.type: Easing.InOutQuad }
                    PropertyAction { target: cell; property: "GridView.delayRemove"; value: false }
                    PropertyAction { target: groupRoot; property: "state"; value: "unclipped" }
                }

            }
        ]
    } // wrapper
} // cell
