import QtQuick 1.1

Item {
    ListView {
        id: socialAuthorization
        delegate: Item {
            Image {
                source: "plugin://" + icon
            }
            Text {
                text: name
            }
        }
    }
}

