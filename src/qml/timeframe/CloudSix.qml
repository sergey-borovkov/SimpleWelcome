import QtQuick 1.0

Item{
    id: cloudSix
    property date cloudDate
    property variant model
    property int margin: 10

    Item {
        id: dateLabel
        anchors.top : parent.top
        anchors.topMargin: parent.height/6
        anchors.left: parent.left
        height: parent.height/10
        width: parent.width/3
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
        id: cloudRect1
        anchors.top: dateLabel.bottom
        anchors.left: parent.left
        width: dateLabel.width
        height: parent.height*3/5
    }    

    CloudRect {
        id: cloudRect2
        anchors.top: parent.top
        anchors.left: cloudRect1.right
        anchors.leftMargin: margin
        height: parent.height*5/9
        width: cloudRect1.width
    }

    CloudRect {
        id: cloudRect3
        anchors.left: cloudRect2.right
        anchors.leftMargin: margin        
        anchors.bottom: cloudRect2.bottom
        width: parent.width/4
        height: parent.height*4/9
    }

    CloudRect {
        id: cloudRect4
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect1.right
        anchors.leftMargin: margin
        width: parent.width*2/9
    }

    CloudRect {
        id: cloudRect5
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect4.right
        anchors.leftMargin: margin
        width: cloudRect4.width
    }

    CloudRect {
        id: cloudRect6
        anchors.top: cloudRect2.bottom
        anchors.topMargin: margin*2
        anchors.bottom: parent.bottom
        anchors.left: cloudRect5.right
        anchors.leftMargin: margin
        width: cloudRect4.width
    }

    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
            {cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 ); \
             cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 ); \
             cloudRect3.image.source = "image://preview/" + model.url(2) + "%" + Math.random( 10 ); \
             cloudRect4.image.source = "image://preview/" + model.url(3) + "%" + Math.random( 10 ); \
             cloudRect5.image.source = "image://preview/" + model.url(4) + "%" + Math.random( 10 ); \
             cloudRect6.image.source = "image://preview/" + model.url(5) + "%" + Math.random( 10 ); \
             }}', cloudSix);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
        cloudRect2.image.source = "image://preview/" + model.url(1) + "%" + Math.random( 10 )
        cloudRect2.url = model.url(1)
        cloudRect3.image.source = "image://preview/" + model.url(2) + "%" + Math.random( 10 )
        cloudRect3.url = model.url(2)
        cloudRect4.image.source = "image://preview/" + model.url(3) + "%" + Math.random( 10 )
        cloudRect4.url = model.url(3)
        cloudRect5.image.source = "image://preview/" + model.url(4) + "%" + Math.random( 10 )
        cloudRect5.url = model.url(4)
        cloudRect6.image.source = "image://preview/" + model.url(5) + "%" + Math.random( 10 )
        cloudRect6.url = model.url(5)
    }
}
