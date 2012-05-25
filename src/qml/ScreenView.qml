import QtQuick 1.1

Item {
    id: screenView

    width: parent.width // probably not needed
    height: parent.height // probably not needed
    anchors.fill: parent

    property int cols: 7
    property int rows: 4

    property int screensTotal: 0
    property int currentScreen: 1

    property variant _screenGridComp
    property variant _lastScreenGrid

    property bool preloaded: false

    property bool arrowsEnabled: false

    Component.onCompleted: preload();

    function preload()
    {
        if(preloaded)
          return;

        _screenGridComp = Qt.createComponent("ScreenGrid.qml");
        if(_screenGridComp.status == Component.Ready)
        {
            init();
        }
        else if(_screenGridComp.status == Component.Error)
        {
            console.log("Component loading error: " + _screenGridComp.errorString());
        }

        preloaded = true;
    }

    function init()
    {
    }

    function addObject(component, opts)
    {
        if(! preloaded)
          preload();

        if(screensTotal == 0)
          _addScreen();

        if(_lastScreenGrid.full())
          _addScreen();

        return _lastScreenGrid.addObject(component, opts);
    }

    function setScreen(screenNum)
    {
        if(screenNum > screensTotal || screenNum < 1)
          return;

        var offset = parent.width * (screenNum - 1);

        setScreenAnimation.stop();
        setScreenAnimation.from = screenViewFlickable.contentX;
        setScreenAnimation.to = offset;
        setScreenAnimation.start();
        //screenViewFlickable.contentX = offset;

        currentScreen = screenNum;
    }

    function clear()
    {
        for(var i = 0; i < screenViewContainer.children.length; i++)
        {
            screenViewContainer.children[i].destroy();
        }

        screensTotal = 0;
        currentScreen = 1;
        arrowsEnabled = false;
    }

    function _addScreen()
    {
        _lastScreenGrid = _screenGridComp.createObject(screenViewContainer, {"cols": cols, "rows": rows, "width": width, "height": height});

        screensTotal += 1;

        if(screensTotal > 1)
          arrowsEnabled = true;
    }

    Flickable {
        id: screenViewFlickable
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds

        flickableDirection: Flickable.HorizontalFlick
        interactive: false

        SmoothedAnimation {
            id: setScreenAnimation
            properties: "contentX"
            target: screenViewFlickable
            //from: 0
            //to: parent.width
            duration: 300
        }

        Row {
            id: screenViewContainer
            anchors.fill: parent
        }
    }

    Rectangle {
        id: leftArrow

        width: 48
        height: 256

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        color: "#113"
        opacity: 0.0
        border.color: "black"
        border.width: 1
        radius: 1

        z: 10

        states: [
            State {
                name: "VISIBLE"
                PropertyChanges { target: leftArrow; opacity: 0.8 }
            },
            State {
                name: "HIDDEN"
                PropertyChanges { target: leftArrow; opacity: 0.0 }
            }
        ]

        transitions: Transition {
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuad }
        }

        MouseArea {
            id: leftArrowMouseArea

            anchors.fill: parent

            onClicked: {
                screenView.setScreen(screenView.currentScreen - 1);
            }
        }
    }

    Rectangle {
        id: rightArrow

        width: 48
        height: 256

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        color: "#113"
        opacity: 0.0
        border.color: "black"
        border.width: 1
        radius: 1

        z: 10

        states: [
            State {
                name: "VISIBLE"
                PropertyChanges { target: rightArrow; opacity: 0.8 }
            },
            State {
                name: "HIDDEN"
                PropertyChanges { target: rightArrow; opacity: 0.0 }
            }
        ]

        transitions: Transition {
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutQuad }
        }

        MouseArea {
            id: rightArrowMouseArea

            anchors.fill: parent

            onClicked: {
                screenView.setScreen(screenView.currentScreen + 1);
            }
        }
    }

    MouseArea {
        id: leftArrowHoverMouseArea

        anchors.left: parent.left
        width: 92
        height: parent.height

        hoverEnabled: true

        onEntered: { if(screenView.arrowsEnabled) leftArrow.state = "VISIBLE" }
        onExited: { leftArrow.state = "HIDDEN" }
    }

    MouseArea {
        id: rightArrowHoverMouseArea

        anchors.right: parent.right
        width: 92
        height: parent.height

        hoverEnabled: true

        onEntered: { if(screenView.arrowsEnabled) rightArrow.state = "VISIBLE" }
        onExited: { rightArrow.state = "HIDDEN" }
    }
}
