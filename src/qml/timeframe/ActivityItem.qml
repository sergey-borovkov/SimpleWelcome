import QtQuick 1.1

Item {
    id: activityItem

    property string path;

    function getFileName( filePath ) {
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
                width: Math.min( sourceSize.width, parent.width )
                height: Math.min( sourceSize.height, parent.height )
                fillMode: Image.PreserveAspectFit

                asynchronous: true
                smooth: true
            }
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
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')
                anchors.centerIn: img
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
                text:  mimeType + " " + activityItem.num + ( ( typeItem == 1 ) ? 'x1 ' : ( typeItem == 2 ) ? 'x2 ' : 'x3 ')
                anchors.centerIn: img
            }
        }
    }

    function bestDelegate( mt ) {
        if ( mt === "image" ) {
            return imgDelegate
        }
        else if ( mt === "video" ) {
            return videoDelegate
        }

        return docDelegate
    }

    Loader {
        id: eventLoader
        anchors.fill: parent
        sourceComponent: bestDelegate ( "image" )
        anchors.margins: 2
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.openUrlExternally(path)
    }
}
