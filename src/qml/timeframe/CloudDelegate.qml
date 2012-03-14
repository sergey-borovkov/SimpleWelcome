
import QtQuick 1.1

Item {
    id: cloudDelegate

    property int activityindex: -1

    function createObjects()
    {
        if ( activityindex === ( -1 ) )
            return;

        var v = activity
        var strDate = Qt.formatDate(activity.getSetDate( activityindex ), "dd-MM-yyyy")
        activityDate.text = strDate

        var acitivityItemComp = Qt.createComponent( "ActivityItem.qml" );

        for ( var i = 0; i < v.getSetCount( activityindex ); i++)
        {
            var url = activity.getUrl( activityindex, i )
            var item = acitivityItemComp.createObject( cloudContainer, { "path": url } );
        }

        /*
        var v = activity
        var y = 0

        Qt.createQmlObject("import QtQuick 1.1; Text { y: " + y + ";text:'" + Qt.formatDate(activity.getSetDate( activityindex ), "dd-MM-yyyy") + "'}", cloudDelegate, "cloudDelegate")

        for(var j = 0; j < v.getSetCount( activityindex ); j++)
        {
            y += 10
            Qt.createQmlObject("import QtQuick 1.1; Text { y:" + y + ";text: '" + activity.getUrl(activityindex, j) + "'}", cloudDelegate, "cloudDelegate")
        }*/
    }

    Component.onCompleted:
    {
        createObjects()
    }

    width: cloudWidth
    height: cloudHeight

    Rectangle {
        id: cloudWrapper
        anchors.fill: parent
        border.color: "blue"
        anchors.margins: 0

        Text {
            id: activityDate
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 5
            anchors.leftMargin: 5
            text: ""
        }

        Grid {
            id: cloudContainer
            spacing: ( parent.width - 100 * columns ) / ( columns + 1 )
            columns: 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.top: activityDate.bottom
            anchors.topMargin: 5
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
        }
    }
}
