import QtQuick 1.1

FocusScope {
    id: applicationsTab
    width: parent.width
    clip: true
    anchors.topMargin: 16
    //focus: true

    AppsGridView {
        id: appsGrid
        anchors.fill: parent
        model: appsGridModel
        interactive: true
        focus: true
        keyNavigationWraps: true


/*        states: State {
            name: "active"; when: appsGrid.activeFocus
            PropertyChanges { target: gridSelection; color: "#FCFFF5"; }
        }

        transitions: Transition {
            NumberAnimation { properties: "scale"; duration: 100 }
        }*/

    }
}
