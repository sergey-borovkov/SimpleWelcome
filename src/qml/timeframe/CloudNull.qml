import QtQuick 1.1

Item{
    id: cloudNone
    property date cloudDate
    property variant model

    Item {
        anchors.centerIn: parent
        width: parent.width
        height: 200
        Text{
            id: dateLabel
            anchors.top : parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 60
            text: Qt.formatDate( cloudDate , "MMM yyyy")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }
        AnimatedImage {
            id: waitIndicator
            anchors.top: dateLabel.bottom
            source: "images/ajax-loader.gif"
            anchors.centerIn: parent
        }
    }
    function createConnection()
    {
        return
//        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
//                        {cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 )}}',
//                                           cloudOne);
//        cloudRect1.image.source = "image://preview/" + model.url(0) + "/rounded" +"%" + Math.random( 10 )
//        cloudRect1.url = model.url(0)
    }





    /*
    CloudRect
    {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
    }


    */
}
