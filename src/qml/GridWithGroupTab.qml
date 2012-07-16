import QtQuick 1.1

Flickable {
    id: flick
    anchors.fill: parent
    contentWidth: parent.width
    contentHeight: gridsContainer.height + 32
    boundsBehavior: Flickable.StopAtBounds // if flicking is not bound, scroll sometimes go crazy and flick far far away from corners when scrolling with mouse wheel
    flickableDirection: Flickable.VerticalFlick

    Behavior on contentY {
        NumberAnimation { duration: 150 }
    }

    function gridsSelectionChanged(obj)
    {
        if (moving)
            return

        var itemHere = mapFromItem(obj, 0, 0)
        var itemInContainer = mapToItem(gridsContainer, itemHere.x, itemHere.y)

        if (itemInContainer.y + 200 > height + contentY)
            contentY += itemHere.y - height + 200
        else if(itemInContainer.y < contentY)
        {
            if (itemInContainer.y < 200)
                contentY = 0
            else
                contentY += itemHere.y
        }
    }

    Component.onCompleted: {
        gridsContainer.gridCurrentItemChanged.connect(gridsSelectionChanged)
    }

    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }
}
