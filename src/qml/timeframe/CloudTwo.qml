import QtQuick 1.0

Item{
    id: cloudTwo
    property date cloudDate
    property variant model

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
    Item {
        anchors.top : parent.top
        anchors.left: parent.left
        anchors.right: cloudRect2.left
        anchors.rightMargin: 10
        anchors.bottom: cloudRect1.top

        Text {
            id: dateLabel
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: labelHeight
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
    }
    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
            {cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 ); \
             cloudRect2.image.source = "image://preview/" + model.url(1) + "/rounded" + "%" + Math.random( 10 ) }}',
            cloudTwo);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
        cloudRect2.image.source = "image://preview/" + model.url(1) + "/rounded" + "%" + Math.random( 10 )
        cloudRect2.url = model.url(1)
    }
}
