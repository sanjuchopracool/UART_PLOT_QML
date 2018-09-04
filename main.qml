import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 320
    title: qsTr("UART PLOT")

    Material.theme: Material.Dark
    minimumWidth: setting.implicitWidth
    minimumHeight: setting.implicitHeight

    TextArea {
        id : outputArea
        anchors.left: buttonBar.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        readOnly: true
        selectByMouse: true
        selectByKeyboard: true

        text: "Hello India"
    }

    Pane {
        id: portSettingPage
        anchors.top: parent.top
        state: 'hide'

        states: [
            State {
                name: 'hide'

                AnchorChanges {
                    target: portSettingPage
                    anchors.left: undefined
                    anchors.right: buttonBar.left
                }
            },

            State {
                name: 'show'

                AnchorChanges {
                    target: portSettingPage
                    anchors.left: buttonBar.right
                    anchors.right: undefined
                }
            }
        ]

        transitions: [
            Transition {
                AnchorAnimation {
                    duration: 200
                }
            }
        ]

        PortSettingItem {
            id: setting
            anchors.fill: parent
        }
    }

    Page {
        id : buttonBar

        height: parent.height

        implicitWidth: columnLayout.implicitWidth
        implicitHeight: columnLayout.implicitHeight

        signal toggle

        onToggle:
        {
            portSettingPage.state = portSettingPage.state === 'show' ? 'hide' : 'show'
        }

        ColumnLayout {
            id : columnLayout

            VerticalButton {
                id : button1
                checkable: true
                text: "Port Setting"

                onClicked: {
                    buttonBar.toggle()
                }
            }
        }
    }
}
