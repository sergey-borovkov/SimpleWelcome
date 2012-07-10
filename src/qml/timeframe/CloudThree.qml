import QtQuick 1.0

Item{
    id: cloudThree
    property date cloudDate
    property variant model
    property int margin: 10


    CloudRect {
        id: cloudRect1
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height*3/5
    }

    CloudRect {
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 - margin
        height: parent.height*4/5
    }

    CloudRect {
        id: cloudRect3
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.left
        anchors.rightMargin: margin
        width: parent.width*1/4
    }

    Item {
        anchors.top : cloudRect2.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: cloudRect3.right
        anchors.leftMargin: margin

        Text{
            id: dateLabel
            anchors.centerIn: parent
            width: 100
            height: labelHeight
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
    }
    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
            {cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 ); \
             cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 ); \
             cloudRect3.image.source = "image://preview/" + model.url(2) + "%" + Math.random( 10 ); \
             }}', cloudThree);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
        cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 )
        cloudRect2.url = model.url(1)
        cloudRect3.image.source = "image://preview/" + model.url(2) + "%" + Math.random( 10 )
        cloudRect3.url = model.url(2)
    }
}
