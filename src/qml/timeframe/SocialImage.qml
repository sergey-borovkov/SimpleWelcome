import QtQuick 1.1

Image{
    id: mainImage

    Image {
        id: waitIndicator
        anchors.centerIn: parent
        source: "images/indicator-shadow.png"
        opacity: 0
        Image {
            id: can
            source: "images/can2.png"
            smooth: true
        }
        Behavior on opacity { NumberAnimation{ duration: 1000 }}
    }
    Timer {
        id: waitTimer
        interval: 10; running: false; repeat: true
        onTriggered: can.rotation = can.rotation +10
    }

    states: [
        State {
            name: "loading"; when: mainImage.status === Image.Loading
            PropertyChanges { target: waitIndicator; visible: true }
            PropertyChanges { target: waitTimer; running: true }
        },
        State {
            name: "error"; when: mainImage.status === Image.Error
            PropertyChanges { target: mainImage; source: 'images/file_broken.png' }
        }
    ]
}
