import QtQuick 1.0

Item{
    id: cloudFour
    property date cloudDate
    property variant model
    property int margin: 10


    CloudRect {
        id: cloudRect1
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width*4/7
        height: parent.height*3/5
    }

    Item {
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: parent.height/10
        Text{
            anchors.centerIn: parent
            width: 100
            height: dateLabel.height
            text: Qt.formatDate( cloudDate , "dd MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    CloudRect {
        id: cloudRect2
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        anchors.right: cloudRect1.left
        anchors.rightMargin: margin
        height: cloudRect1.height - dateLabel.height
    }

    CloudRect {
        id: cloudRect3
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.right: cloudRect2.right
        width: parent.width*2/7
    }

    CloudRect {
        id: cloudRect4
        anchors.top: cloudRect1.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.left
        width: cloudRect3.width
    }

    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
            {cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 ); \
             cloudRect2.image.source = "image://preview/" + model.url(1) + "/rounded" + "%" + Math.random( 10 ); \
             cloudRect3.image.source = "image://preview/" + model.url(2) + "/rounded" + "%" + Math.random( 10 ); \
             cloudRect4.image.source = "image://preview/" + model.url(3) + "/rounded" + "%" + Math.random( 10 ); \
             }}', cloudFour);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
        cloudRect2.image.source = "image://preview/" + model.url(1) + "/rounded" + "%" + Math.random( 10 )
        cloudRect2.url = model.url(1)
        cloudRect3.image.source = "image://preview/" + model.url(2) + "/rounded" + "%" + Math.random( 10 )
        cloudRect3.url = model.url(2)
        cloudRect4.image.source = "image://preview/" + model.url(3) + "/rounded" + "%" + Math.random( 10 )
        cloudRect4.url = model.url(3)
    }
}
