
import QtQuick 1.1



Rectangle {
    id: cloudDelegate

    color: "transparent"
    property int idx: 0

    property int k:  width / 27
    property int x1: 3*k
    property int x2: 4*k
    property int x3: 6*k
    property int y1: 2*k
    property int y2: 3*k
    property int y3: 4*k

    width: cloudWidth
    height: cloudHeight

    function getPath( ind )
    {
        //console.log("path" + idx + " -- " + activity.getUrl( idx, ind ))

        return activity.getUrl( idx, ind )
    }

    function getDate()
    {
        return ( "<b>" + Qt.formatDate( activity.getSetDate( idx ), "dd-MM-yyyy" ) + "</b>" )
    }

    function createItem( component, url, type )
    {
        return component.createObject( cloudDelegate, { "path": url,
                                                        "width": ( ( type == 1 ) ? x1 : ( ( type == 2 ) ? x2 : x3 ) ),
                                                        "height": ( ( type == 1 ) ? y1 : ( ( type == 2 ) ? y2 : y3 ) ) } );
    }

    function createTemplate1( component, date )
    {
        //console.log("******************* createTemplate 1 ******************")

        var item = createItem( component, getPath( 0 ), 3 )

        item.x = ( width - item.width ) / 2
        item.y = ( height - item.height ) / 2

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = item.x
        dateItem.y = item.y - k - dateItem.height
    }

    function createTemplate2( component, date )
    {
        //console.log("******************* createTemplate 2 ******************")
        var types = new Array( 3, 2 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 0 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + k ) ) / 2
        items[ 0 ].y = ( height - items[ 0 ].height ) / 2

        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }

    function createTemplate3( component, date )
    {
        //console.log("******************* createTemplate 3 ******************")

        var types = new Array( 3, 2, 1 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 0 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + k ) ) / 2
        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 2 ].x = ( width - items[ 2 ].width ) / 2
        items[ 0 ].y = ( height - ( items[ 0 ].height + items[ 2 ].height + k ) ) / 2
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )
        items[ 2 ].y = items[ 0 ].y + items[ 0 ].height + k

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }

    function createTemplate4( component, date )
    {
        //console.log("******************* createTemplate 4 ******************")

        var types = new Array( 3, 2, 1, 1 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 0 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + k ) ) / 2
        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 2 ].x = ( width - items[ 2 ].width ) / 2
        items[ 3 ].x = items[ 0 ].x
        items[ 0 ].y = ( height - ( items[ 0 ].height + items[ 2 ].height + k ) ) / 2
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )
        items[ 2 ].y = items[ 0 ].y + items[ 0 ].height + k
        items[ 3 ].y = items[ 2 ].y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }

    function createTemplate5( component, date )
    {
        //console.log("******************* createTemplate 5 ******************")

        var types = new Array( 3, 2, 1, 1, 1 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 0 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + k ) ) / 2
        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 2 ].x = ( width - items[ 2 ].width ) / 2
        items[ 3 ].x = items[ 0 ].x
        items[ 4 ].x = items[ 2 ].x + items[ 2 ].width + k
        items[ 0 ].y = ( height - ( items[ 0 ].height + items[ 2 ].height + k ) ) / 2
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )
        items[ 2 ].y = items[ 0 ].y + items[ 0 ].height + k
        items[ 3 ].y = items[ 2 ].y
        items[ 4 ].y = items[ 2 ].y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }

    function createTemplate6( component, date )
    {
        //console.log("******************* createTemplate 6 ******************")

        var types = new Array( 3, 2, 1, 1, 1, 3 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 5 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + items[ 5 ].width + k * 2 ) ) / 2
        items[ 0 ].x = items[ 5 ].x + items[ 0 ].width + k
        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 3 ].x = items[ 0 ].x
        items[ 2 ].x = items[ 3 ].x + items[ 3 ].width + k
        items[ 4 ].x = items[ 2 ].x + items[ 2 ].width + k
        items[ 0 ].y = ( height - ( items[ 0 ].height + items[ 2 ].height + k ) ) / 2
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )
        items[ 2 ].y = items[ 0 ].y + items[ 0 ].height + k
        items[ 3 ].y = items[ 2 ].y
        items[ 4 ].y = items[ 2 ].y
        items[ 5 ].y = items[ 0 ].y + (items[ 0 ].height + items[ 3 ].height + k - items[ 5 ].height ) / 2

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }

    function createTemplate7( component, date )
    {
        //console.log("******************* createTemplate 7 ******************")

        var types = new Array( 3, 2, 1, 1, 1, 3, 3 )
        var items = new Array();

        for ( var i = 0; i < types.length; i++ )
            items[ i ] = createItem( component, getPath( i ), types[ i ] )

        items[ 5 ].x = ( width - ( items[ 0 ].width + items[ 1 ].width + items[ 5 ].width + items[ 6 ].width + k * 3 ) ) / 2
        items[ 0 ].x = items[ 5 ].x + items[ 0 ].width + k
        items[ 1 ].x = items[ 0 ].x + items[ 0 ].width + k
        items[ 3 ].x = items[ 0 ].x
        items[ 2 ].x = items[ 3 ].x + items[ 3 ].width + k
        items[ 4 ].x = items[ 2 ].x + items[ 2 ].width + k
        items[ 6 ].x = items[ 1 ].x + items[ 1 ].width + k
        items[ 0 ].y = ( height - ( items[ 0 ].height + items[ 2 ].height + k ) ) / 2
        items[ 1 ].y = items[ 0 ].y + ( items[ 0 ].height - items[ 1 ].height )
        items[ 2 ].y = items[ 0 ].y + items[ 0 ].height + k
        items[ 3 ].y = items[ 2 ].y
        items[ 4 ].y = items[ 2 ].y
        items[ 5 ].y = items[ 0 ].y + (items[ 0 ].height + items[ 3 ].height + k - items[ 5 ].height ) / 2
        items[ 6 ].y = items[ 5 ].y

        var dateItem = Qt.createQmlObject("import QtQuick 1.1; Text { text:'" + date + "'}", cloudDelegate, "cloudDelegate")
        dateItem.x = items[ 1 ].x
        dateItem.y = items[ 1 ].y - k - dateItem.height
    }


    function createObjects()
    {
        var strDate = getDate()
        var acitivityItemComp = Qt.createComponent( "ActivityItem.qml" );

        var activityCount = activity.getSetCount( idx )
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

}
