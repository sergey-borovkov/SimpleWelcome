import QtQuick 1.0

// This compoment has few ugly workarounds because of Qt QTBUG-14900
// because of this bug it's very hard to update image using image provider
// so we add some random symbols to file name every time we are interested in image
// update

Image {
    source: "image://preview/" + filePath + "%" + Math.random( 10 )
    width: Math.min( sourceSize.width, parent.width )
    height: Math.min( sourceSize.height, parent.height )
    fillMode: Image.PreserveAspectFit
    anchors.centerIn: parent
    smooth: true

    Connections
    {
        target: fileModel
        onDataChanged:
        {
            source = "image://preview/" + filePath + "%" + Math.random( 10 )
        }
    }
}
