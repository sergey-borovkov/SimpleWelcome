// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: activityItem
    property string source: ""
    width: 64; height: 64
    Image {
        width: 64; height: 64
        anchors.centerIn: parent
        source: activityItem.source

    }

}
