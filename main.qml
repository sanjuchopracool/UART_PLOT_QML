import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 640
    height: 320
    title: qsTr("UART PLOT")

    Material.theme: Material.Dark
    minimumWidth: setting.implicitWidth
    minimumHeight: setting.implicitHeight

    TextArea {
        id : outputArea
        anchors.left: portSettingPage.right
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
        readonly property bool hidden: {
            state === "hide"
        }

        state: portPageSetting.state

        function toggleState() {
            portSettingPage.state = portSettingPage.state === "show" ? "hide" : "show";
        }

        states: [
            State {
                name: 'hide'

                AnchorChanges {
                    target: portSettingPage
                    anchors.left: undefined
                    anchors.right: buttonBar.right
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

        Settings {
            id: portPageSetting
            property string state: "show"
        }

        Component.onDestruction: {
            portPageSetting.state = portSettingPage.state
        }
    }

    Page {
        id : buttonBar

        height: parent.height

        implicitWidth: columnLayout.implicitWidth
        implicitHeight: columnLayout.implicitHeight

        ColumnLayout {
            id : columnLayout

            VerticalButton {
                id : portSettingButton
                checkable: true
                text: "Port Setting"

                onClicked: {
                    portSettingPage.toggleState()
                }
            }
        }
    }


    Settings {
        property alias x: appWindow.x
        property alias y: appWindow.y
        property alias width: appWindow.width
        property alias height: appWindow.height
    }

    Component.onCompleted: {
        portSettingButton.checked = portSettingPage.hidden
    }
}
