import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width
    clip: true

    property variant grid: appsGrid

    IconGridView {
        id: appsGrid
        anchors.fill: parent

        model: appsModel
        interactive: false
        focus: true
        keyNavigationWraps: true
        //flow: GridView.TopToBottom

        ListModel
        {
            id: appsModel

            signal itemClicked(int newIndex)
        }
        function onItemClicked(newIndex)
        {
            appsGridModel.itemClicked(newIndex == -1 ? newIndex : appsModel.get(newIndex).id)
        }

        function newItemData(iconPath, name)
        {
            //console.log("New Entry | " + iconPath + " | " + name + " | " + appsModel.count)
            appsModel.append( { imagePath: iconPath, caption: name, id: appsModel.count})
        }


        function appsContentChanged() {
            appsGrid.currentIndex = 0
            appsGrid.highlightItem.opacity = 1
        }

        function dataClear() {
            appsModel.clear()
        }

        Component.onCompleted: {
            appsGridModel.contentChanged.connect(appsContentChanged)
            appsGridModel.newItemData.connect(newItemData)
            appsModel.itemClicked.connect(onItemClicked)
            appsGridModel.dataClear.connect(dataClear)
            appsGridModel.updateContent()
        }

        anchors.top: parent.top
        anchors.topMargin: 32
    }

}
