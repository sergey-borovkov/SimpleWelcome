import QtQuick 1.1

Item {
    id: applicationsTab
    width: parent.width
    clip: true
    anchors.topMargin: 16

    AppsGridView {
        anchors.fill: parent
        model: appsGridModel
    }
}
