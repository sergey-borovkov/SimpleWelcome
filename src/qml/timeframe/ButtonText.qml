// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Text {

    anchors.verticalCenter: parent.verticalCenter
    anchors.leftMargin: 16
    anchors.rightMargin: 16
    anchors.topMargin: 8

    maximumLineCount: 1
    clip: true
    elide: Text.ElideRight
    wrapMode: Text.Wrap

    font.pointSize: 14
    font.family: "Bitstream Vera Sans"

    style: Text.Raised
    styleColor: "#000"
    color: "#eee"

}
