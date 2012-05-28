import QtQuick 1.1

Item {
    id: testingTab
    width: parent.width

    clip: true
    anchors.topMargin: 16

    property variant _buttonComp

    Component.onCompleted: preload();

    function preload()
    {
        _buttonComp = Qt.createComponent("Button.qml");
        if(_buttonComp.status == Component.Ready)
        {
            init();
        }
        else if(_buttonComp.status == Component.Error)
        {
            console.log("Component loading error: " + _buttonComp.errorString());
        }
    }

    function init()
    {
        reload();
    }

    function reload()
    {
        for(var i = 0; i < 100; i++)
        {
            //console.log("Iterating: " + i);

            var button = screenView.addObject(_buttonComp,
            {
                "label": "TESTING " + i,
                "iconUrl": "image://generalicon/stock/inode-directory.png",
                "clickCallbackData": { "bla": "blabla" }
            });

            function createCallback(data)
            {
                var storeData = data;

                function callback()
                {
                    console.log("DATADATA" + storeData.bla);
                }

                return callback;
            }

            button.buttonClick.connect(createCallback(button.clickCallbackData));
        }
    }

    ScreenView
    {
        id: screenView

        cols: 7
        rows: 4
    }


}
