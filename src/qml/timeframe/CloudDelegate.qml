
import QtQuick 1.1



Item {
    id: cloudDelegate

    property int activityindex: -1

    property int k:  width / 27  //10*2
    property int x1: 3*k
    property int x2: 4*k
    property int x3: 6*k
    property int y1: 2*k
    property int y2: 3*k
    property int y3: 4*k


    //    width: x3 * 3 + x2 + k * 7
    //    height: timeFrameTab.height //y1 + y3 + k * 5

    width: cloudWidth
    height: cloudHeight

    function getPath( idx )
    {
        return activity.getUrl( activityindex, idx )
    }

    function getDate()
    {
        if ( activityindex === ( -1 ) )
            return ""

        return ( "<b>" + Qt.formatDate( activity.getSetDate( activityindex ), "dd-MM-yyyy" ) + "</b>" )
    }

    function createItem( component, url, type )
    {
        return component.createObject( cloudDelegate, { "path": url,
                                                        "width": ( ( type == 1 ) ? x1 : ( ( type == 2 ) ? x2 : x3 ) ),
                                                        "height": ( ( type == 1 ) ? y1 : ( ( type == 2 ) ? y2 : y3 ) ) } );
    }

    function createTemplate1( component, date )
    {
        var url = getPath( 0 )
        var item = component.createObject( cloudDelegate, { "path": url, "width": x3, "height": y3 } );

        item.x = ( width - item.width ) / 2
        item.y = ( height - item.height ) / 2

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item.x
        dateItem.y = item.y - k - dateItem.height
    }

    function createTemplate2( component, date )
    {
        var urls = new Array[2];
        for ( i = 0; i < 2; i++)
            urls[ i ] = getPath( i )

//        var items = new Array[2];
//        for ( i = 0; i < 2; i++)
//            items[ i ] = getPath( i )

        var item1 = createItem( component, urls[ 0 ], 3 )
        var item2 = createItem( component, urls[ 1 ], 2 )

        item1.x = ( width - ( item1.width + item2.width + k) ) / 2
        item2.x = item1.x + item1.width + k
        item1.y = ( height - item1.height ) / 2
        item2.y = item1.y + (item1.height - item2.height)

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }

    function createTemplate3( component, date )
    {
        var url1 = getPath( 0 )
        var url2 = getPath( 1 )
        var url3 = getPath( 2 )

        var item1 = component.createObject( cloudDelegate, { "path": url1, "width": x3, "height": y3 } );
        var item2 = component.createObject( cloudDelegate, { "path": url2, "width": x2, "height": y2 } );
        var item3 = component.createObject( cloudDelegate, { "path": url3, "width": x1, "height": y1 } );

        item1.x = ( width - ( item1.width + item2.width + k ) ) / 2
        item2.x = item1.x + item1.width + k
        item3.x = ( width - item3.width ) / 2
        item1.y = ( height - ( item1.height + item3.height + k ) ) / 2
        item2.y = item1.y + ( item1.height - item2.height )
        item3.y = item1.y + item1.height + k

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }

    function createTemplate4( component, date )
    {
        var url1 = getPath( 0 )
        var url2 = getPath( 1 )
        var url3 = getPath( 2 )
        var url4 = getPath( 3 )

        var item1 = component.createObject( cloudDelegate, { "path": url1, "width": x3, "height": y3 } );
        var item2 = component.createObject( cloudDelegate, { "path": url2, "width": x2, "height": y2 } );
        var item3 = component.createObject( cloudDelegate, { "path": url3, "width": x1, "height": y1 } );
        var item4 = component.createObject( cloudDelegate, { "path": url4, "width": x1, "height": y1 } );

        item1.x = ( width - ( item1.width + item2.width + k ) ) / 2
        item2.x = item1.x + item1.width + k
        item3.x = ( width - item3.width ) / 2
        item4.x = item1.x
        item1.y = ( height - ( item1.height + item3.height + k ) ) / 2
        item2.y = item1.y + ( item1.height - item2.height )
        item3.y = item1.y + item1.height + k
        item4.y = item3.y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }

    function createTemplate5( component, date )
    {
        var url1 = getPath( 0 )
        var url2 = getPath( 1 )
        var url3 = getPath( 2 )
        var url4 = getPath( 3 )
        var url5 = getPath( 4 )

        var item1 = component.createObject( cloudDelegate, { "path": url1, "width": x3, "height": y3 } );
        var item2 = component.createObject( cloudDelegate, { "path": url2, "width": x2, "height": y2 } );
        var item3 = component.createObject( cloudDelegate, { "path": url3, "width": x1, "height": y1 } );
        var item4 = component.createObject( cloudDelegate, { "path": url4, "width": x1, "height": y1 } );
        var item5 = component.createObject( cloudDelegate, { "path": url5, "width": x1, "height": y1 } );

        item1.x = ( width - ( item1.width + item2.width + k ) ) / 2
        item2.x = item1.x + item1.width + k
        item3.x = ( width - item3.width ) / 2
        item4.x = item1.x
        item5.x = item3.x + item3.width + k
        item1.y = ( height - ( item1.height + item3.height + k ) ) / 2
        item2.y = item1.y + ( item1.height - item2.height )
        item3.y = item1.y + item1.height + k
        item4.y = item3.y
        item5.y = item3.y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }

    function createTemplate6( component, date )
    {
        var url1 = getPath( 0 )
        var url2 = getPath( 1 )
        var url3 = getPath( 2 )
        var url4 = getPath( 3 )
        var url5 = getPath( 4 )
        var url6 = getPath( 5 )

        var item1 = component.createObject( cloudDelegate, { "path": url1, "width": x3, "height": y3 } );
        var item2 = component.createObject( cloudDelegate, { "path": url2, "width": x2, "height": y2 } );
        var item3 = component.createObject( cloudDelegate, { "path": url3, "width": x1, "height": y1 } );
        var item4 = component.createObject( cloudDelegate, { "path": url4, "width": x1, "height": y1 } );
        var item5 = component.createObject( cloudDelegate, { "path": url5, "width": x1, "height": y1 } );
        var item6 = component.createObject( cloudDelegate, { "path": url6, "width": x3, "height": y3 } );

        item6.x = ( width - ( item1.width + item2.width + item6.width + k * 2 ) ) / 2
        item1.x = item6.x + item1.width + k
        item2.x = item1.x + item1.width + k
        item4.x = item1.x
        item3.x = item4.x + item4.width + k
        item5.x = item3.x + item3.width + k
        item1.y = ( height - ( item1.height + item3.height + k ) ) / 2
        item2.y = item1.y + ( item1.height - item2.height )
        item3.y = item1.y + item1.height + k
        item4.y = item3.y
        item5.y = item3.y
        item6.y = item1.y + (item1.height + item4.height + k - item6.height ) / 2

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }

    function createTemplate7( component, date )
    {
        var url1 = getPath( 0 )
        var url2 = getPath( 1 )
        var url3 = getPath( 2 )
        var url4 = getPath( 3 )
        var url5 = getPath( 4 )
        var url6 = getPath( 5 )
        var url7 = getPath( 6 )

        var item1 = component.createObject( cloudDelegate, { "path": url1, "width": x3, "height": y3 } );
        var item2 = component.createObject( cloudDelegate, { "path": url2, "width": x2, "height": y2 } );
        var item3 = component.createObject( cloudDelegate, { "path": url3, "width": x1, "height": y1 } );
        var item4 = component.createObject( cloudDelegate, { "path": url4, "width": x1, "height": y1 } );
        var item5 = component.createObject( cloudDelegate, { "path": url5, "width": x1, "height": y1 } );
        var item6 = component.createObject( cloudDelegate, { "path": url6, "width": x3, "height": y3 } );
        var item7 = component.createObject( cloudDelegate, { "path": url7, "width": x3, "height": y3 } );

        item6.x = ( width - ( item1.width + item2.width + item6.width + item7.width + k * 3 ) ) / 2
        item1.x = item6.x + item1.width + k
        item2.x = item1.x + item1.width + k
        item4.x = item1.x
        item3.x = item4.x + item4.width + k
        item5.x = item3.x + item3.width + k
        item7.x = item2.x + item2.width + k
        item1.y = ( height - ( item1.height + item3.height + k ) ) / 2
        item2.y = item1.y + ( item1.height - item2.height )
        item3.y = item1.y + item1.height + k
        item4.y = item3.y
        item5.y = item3.y
        item6.y = item1.y + (item1.height + item4.height + k - item6.height ) / 2
        item7.y = item6.y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item2.x
        dateItem.y = item2.y - k - dateItem.height
    }


    function createObjects()
    {
        if ( activityindex === ( -1 ) )
            return;

        var strDate = getDate()
        var acitivityItemComp = Qt.createComponent( "ActivityItem.qml" );

        var activityCount = activity.getSetCount( activityindex )
        if ( activityCount == 1 )
            createTemplate1( acitivityItemComp, strDate )
        else if ( activityCount == 2 )
            createTemplate2( acitivityItemComp, strDate )
        else if ( activityCount == 3 )
            createTemplate3( acitivityItemComp, strDate )
        else if ( activityCount == 4 )
            createTemplate4( acitivityItemComp, strDate )
        else if ( activityCount == 5 )
            createTemplate5( acitivityItemComp, strDate )
        else if ( activityCount == 6 )
            createTemplate6( acitivityItemComp, strDate )
        else
            createTemplate7( acitivityItemComp, strDate )

    }

    Component.onCompleted:
    {
        createObjects()
    }

/*
    Rectangle {
        anchors.fill: parent
        border.color: "green"
        border.width: 3
    }
*/
    Item {
        id: cloudItem
        anchors.centerIn: parent
//        anchors.fill: parent
    }

}

/*


Item {
    id: cloudDelegate

    property int k: 10*2
    property int x1: 3*k
    property int x2: 4*k
    property int x3: 6*k
    property int y1: 2*k
    property int y2: 3*k
    property int y3: 4*k

    property int activityindex: -1


    //    width: x3 * 3 + x2 + k * 7
    //    height: timeFrameTab.height //y1 + y3 + k * 5

    width: cloudWidth
    height: cloudHeight

    function getPath( idx )
    {
        return ""
    }


    function getDate()
    {
        if ( activityindex === ( -1 ) )
            return ""

        return ( "<b>" + Qt.formatDate( activity.getSetDate( activityindex ), "dd-MM-yyyy" ) + "</b>" )
    }

    Component.onCompleted:
    {
        updateObjects()
    }

    Item {
        id: cloudItem
        anchors.centerIn: parent

        ActivityItem {
            id: item1
            anchors.horizontalCenter: parent.horizontalCenter
            //            anchors.verticalCenter: parent.verticalCenter
            //            anchors.top: item5.bottom
            typeItem: 1
        }

        ActivityItem {
            id: item2
            typeItem: 1
            anchors.right: item1.left
            anchors.top: item1.top
        }
        ActivityItem {
            id: item3
            typeItem: 1
            anchors.left: item1.right
            anchors.top: item1.top
        }
        ActivityItem {
            id: item4
            typeItem: 2
            anchors.right: item3.right
            anchors.bottom: item3.top
        }
        ActivityItem {
            id: item5
            typeItem: 3
            y: parent.height / 2 - height
            anchors.left: item2.left
        }
        ActivityItem {
            id: item6
            typeItem: 3
            anchors.right: item2.left
            anchors.verticalCenter: parent.verticalCenter

        }
        ActivityItem {
            id: item7
            typeItem: 3
            anchors.left: item3.right
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: date
            text: ""
            anchors.right: item6.right
            anchors.bottom: item6.top
            anchors.rightMargin: k
        }
    }

}
*/
