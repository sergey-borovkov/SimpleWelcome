// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: button
    width: 100
    height: 38
    state: ""

    color: "transparent"
    border.color: "transparent"
    border.width: 1
    radius: 5
    opacity: 0.8

    states: State {
        name: "selected"
        PropertyChanges { target: button; color: "#222" }
        PropertyChanges { target: button; border.color: "black" }
        PropertyChanges { target: button; opacity: 1 }
    }

}
