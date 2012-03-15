// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: activityItem
    property string path;

    function getFileName( filePath )
    {
        var filename = ""
        var pos = filePath.lastIndexOf("/");
        if ( pos !== ( -1 ) )
            filename = filePath.substring( pos + 1 );
        return filename;
    }

    Component {
        id: imgDelegate
        Item {
            id: imgItem
            anchors.centerIn: parent

            Image {
                id: img
                source: "image://preview/" + path
                asynchronous: true
                //sourceSize.width: 256
                width: activityItem.width
                height: activityItem.height

                fillMode: Image.PreserveAspectFit
                smooth: true

                Component.onCompleted:
                {
                    console.log("path is " + path)
                }

            }

/*            Text{
                anchors.top: img.bottom
                anchors.bottom: parent.bottom
                text: getFileName( path )
                elide: Text.ElideRight
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
            }
*/
        }
    }

    Component {
        id: videoDelegate
        Item {
            id: videoItem

            Rectangle {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: activityItem.width
                height: activityItem.height
                color: "red"
                opacity: cloudDelegate.ListView.isCurrentItem ? 1 : 0.5
            }
            Text {
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')//  + url
                anchors.centerIn: img
        //        anchors.top: img.bottom
        //        anchors.topMargin: 5
            }
        }
    }

    Component {
        id: docDelegate
        Item {
            id: docItem
            Rectangle {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: activityItem.width
                height: activityItem.height
                color: "green"
                opacity: cloudDelegate.ListView.isCurrentItem ? 1 : 0.5
            }
            Text {
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')//  + url
                anchors.centerIn: img
        //        anchors.top: img.bottom
        //        anchors.topMargin: 5
            }
        }
    }

    function bestDelegate( mt )
    {
        if ( mt === "image" )
        {
            return imgDelegate;
        }
        else if ( mt === "video" )
        {
            return videoDelegate;
        }
        else
        {
            return docDelegate;
        }
    }

    Loader {
        id: eventLoader
        anchors.fill: parent
        sourceComponent: bestDelegate ( "image" )
        anchors.margins: 2
    }
}
