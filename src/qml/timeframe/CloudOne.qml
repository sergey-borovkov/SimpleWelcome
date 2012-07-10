import QtQuick 1.1

Item{
    id: cloudOne
    property date cloudDate
    property variant model

    Text{
        id: dateLabel
        anchors.top : parent.top
        anchors.left: parent.left
        width: 100
        height: labelHeight
        text: Qt.formatDate( cloudDate , "dd MMM yyyy")
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }
    CloudRect
    {
        id: cloudRect1
        anchors.top: dateLabel.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
    }
    function createConnection()
    {
        var newObject = Qt.createQmlObject('import QtQuick 1.0;  Connections {target: model; onGotThumbnail: \
                        {cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 )}}',
                                           cloudOne);
        cloudRect1.image.source = "image://preview/" + model.url(0) + "%" + Math.random( 10 )
        cloudRect1.url = model.url(0)
    }
}
