import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width
    clip: true

    property variant grid: appsGrid

    AppsGridView {
        id: appsGrid
        anchors.fill: parent
        model: appsGridModel
        interactive: true
        focus: true
        keyNavigationWraps: true

        function appsContentChanged() {
            appsGrid.currentIndex = 0
            appsGrid.highlightItem.opacity = 1
        }

        Component.onCompleted: {
            appsGridModel.contentChanged.connect(appsContentChanged)
        }

        anchors.top: parent.top
        anchors.topMargin: 32
    }
}
