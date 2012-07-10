import QtQuick 1.0

Item{
    id: cloudTwo
    property date cloudDate
    property variant model
    Item {
        anchors.top : parent.top
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height/2

        Text {
            id: dateLabel
            anchors.centerIn: parent
            width: 100
            height: labelHeight
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter            
        }
    }
    CloudRect {
        id: cloudRect1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width*1/3
        height: parent.height/2
    }
    CloudRect {
        id: cloudRect2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width*2/3 -20
        height: parent.height*4/5
    }

    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
            {cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 ); \
             cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 ) }}',
            cloudTwo);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
        cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 )
        cloudRect2.url = model.url(1)
    }
}
