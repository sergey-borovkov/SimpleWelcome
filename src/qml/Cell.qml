import QtQuick 1.1
import Effects 1.0

Item {
    id: cell

    // constants
    property int cellWidth: constants.cellWidth

    width: cellWidth
    height: wrapper.height//140

    GridView.onRemove: {
        wrapper.state = "REMOVING"
    }

    Item {
        id: wrapper
        parent: gridMouseArea
        x: cell.x
        y: cell.y
        width: cellWidth
        height: childrenRect.height

        BorderImage {
            anchors.left: cellIcon.left
            anchors.top: cellIcon.top
            width: cellIcon.width + 5
            height: cellIcon.height + 4
            source: "image://generalicon/asset/stacked_icon_bg.png"
            border.left: 11
            border.top: 11
            border.right: 16
            border.bottom: 15
            visible: {
                if (stack !== undefined)
                    return true
                return false
            }
        }

        Image {
            id: cellIcon
            source: imagePath
            width: constants.iconSize
            height: constants.iconSize
            anchors.top: parent.top
            anchors.topMargin: constants.iconSize / 5 //20
            anchors.horizontalCenter: parent.horizontalCenter

            /*effect: DropShadow {
                    blurRadius: 50
                    color: Qt.rgba(0, 0, 0, 0.3)
                    xOffset: 0.5
                    yOffset: 0.5
                }*/

            smooth: true
        }

        Image {
            id: icon
            anchors.top: cellIcon.top
            anchors.right: cellIcon.right
            width: 16
            height: 16
            z: 1
            source: {
                pinned === true ? "image://generalicon/asset/pinned.png" : ""
            }
        }

        Text {
            id: cellText
            text: caption

            width: parent.width - 10
            anchors.top: cellIcon.bottom
            anchors.topMargin: constants.iconSize / 5 //24
            anchors.horizontalCenter: parent.horizontalCenter

            /*effect: DropShadow {
                    blurRadius: 3
                    color: Qt.rgba(0, 0, 0, 1)
                    xOffset: 0.3
                    yOffset: 0.7
                }*/
            style: Text.Sunken
            styleColor: "#000"
            color: "#eee"
            font.bold: true
            font.family: "Bitstream Vera Sans"
            font.pointSize: constants.iconTextSize

            maximumLineCount: 3
            elide: Text.ElideRight
            clip: true
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }

        Behavior on x { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }
        Behavior on y { enabled: wrapper.state == "gridInDrag"; NumberAnimation { duration: 150; easing.type: Easing.InOutQuad } }

        states: [
            State {
                name: "gridInDrag"
                when: gridMouseArea.dndSrcId != -1 && gridMouseArea.dndSrcId != id
            },

            State {
                name: "cellInDrag"
                when: gridMouseArea.dndSrcId === id

                ParentChange {
                    target: wrapper
                    parent: tabRoot
                    x: gridMouseArea.mapToItem(tabRoot, gridMouseArea.mouseX - wrapper.width/2, 0).x
                    y: gridMouseArea.mapToItem(tabRoot, 0, gridMouseArea.mouseY - wrapper.height/2).y
                    //x: gridMouseArea.mouseX - wrapper.width/2
                    //y: gridMouseArea.mouseY - wrapper.height/2
                }
            },

            State {
                name: "cellOpenedStack"
                when: stackCellOpenedId === id && stack !== undefined

                ParentChange {
                    target: wrapper
                    parent: tabWrapper
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
                    NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
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
                                 PropertyAction { target: gridMouseArea; property: "dndSrcId"; value: -1 }
                                 PropertyAction { target: cell; property: "GridView.delayRemove"; value: false }
                             }

            }
        ]

        Item { // Necessary for correct highlight height
            height: cellIcon.anchors.topMargin + cellIcon.height + cellText.anchors.topMargin + cellText.height + cellIcon.anchors.topMargin/2
        }
    }
}
