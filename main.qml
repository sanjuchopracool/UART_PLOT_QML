import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Universal 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import com.sanjay.serial 1.0

Item {
    id: appWindow
    anchors.fill: parent

    implicitWidth: buttonBar.implicitWidth
    signal stateChanged(string state)
    readonly property int settingPageWidth: {
        portSettingPage.width
    }

    state: portSettingPage.state

    Pane {
        id: portSettingPage
        anchors.top: parent.top
        readonly property bool hidden: {
            state === "hide"
        }

        state: portPageSetting.state

        function toggleState() {
            portSettingPage.state = portSettingPage.state === "show" ? "hide" : "show";
            appWindow.stateChanged(state)
        }

        onWidthChanged: {
            appWindow.stateChanged(state)
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

    Component.onCompleted: {
        portSettingButton.checked = portSettingPage.hidden
    }
}
