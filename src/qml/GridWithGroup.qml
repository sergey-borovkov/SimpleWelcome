import QtQuick 1.1

Column {
    property string groupName
    property variant gridModel
    property variant prevGridGroup
    property variant nextGridGroup

    property bool defaultFocus: false
    property variant gridView: appsGridView
    property int count: appsGridView.count
    signal gridItemCountChanged


    spacing: 16
    width: parent.width
    height: childrenRect.height
    visible: appsGridView.count != 0

    Text {
        text: groupName
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
    }

    AppsGridView {
        id: appsGridView
        model: gridModel

        width: parent.width
        height: Math.ceil(count / columns) * 200

        prevGrid: prevGridGroup.gridView
        nextGrid: nextGridGroup.gridView

        interactive: false
        focus: defaultFocus

        function updateSelection()
        {
            parent.count = count // without this it is updated too late
            if (highlightItem)
            {
                highlightItem.animationDuration = 0
                highlightItem.opacity = 0
                highlightItem.animationDuration = 150
            }
            gridItemCountChanged()
        }

        Component.onCompleted: {
            if ('group' in model)
                model.group = groupName
            updateSelection()
        }

        Behavior on height {
            NumberAnimation { duration: 150 }
        }

        onCountChanged: {
            updateSelection()
        }
    }
}
