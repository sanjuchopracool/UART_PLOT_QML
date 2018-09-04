import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id : verticalButton
    property alias text: actualButton.text
    height: actualButton.implicitWidth
    width: actualButton.implicitHeight
    property alias checkable: actualButton.checkable
    signal clicked
    Button {
        id : actualButton

        onClicked: {
            parent.clicked()
        }
    }

    transform: [
        Rotation{
            id: rotateButton
            angle: -90
            origin.x: actualButton.height/2
            origin.y: actualButton.height/2
        },

        Translate{
            id: translateButton
            y : actualButton.width - actualButton.height
        }
    ]
}
