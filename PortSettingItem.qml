import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.sanjay.serial 1.0

Item {
    implicitWidth: settingLayout.implicitWidth
    implicitHeight: settingLayout.implicitHeight + connectButton.implicitHeight + settingLayout.rowSpacing

    GridLayout
    {
        id: settingLayout
        columns: 4
        anchors.fill: parent
        anchors.bottomMargin: connectButton.implicitHeight + settingLayout.rowSpacing

        /////////////////// Ports ///////////////////
        Label {
            text: qsTr("Port")
        }

        ComboBox {
            id: portsCombo
            model: SerialPortManager.ports
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        ///////////////////  Baud Rate ///////////////////
        Label {
            text: qsTr("Baud rate")
        }

        ComboBox {
            id: baudRateCombo
            model: [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
        }

        ///////////////////  Parity ///////////////////
        Label {
            text: qsTr("Parity")
        }

        ComboBox {
            id: parityCombo
            textRole: "key"
            model: ListModel {
                ListElement {
                    key: "None";
                    value: 0
                }
                ListElement {
                    key: "Even";
                    value: 2
                }
                ListElement {
                    key: "Odd";
                    value: 3
                }
                ListElement {
                    key: "Space";
                    value: 4
                }
                ListElement {
                    key: "Mark";
                    value: 5
                }
            }
        }

        ///////////////////  Data bits ///////////////////
        Label {
            text: qsTr("Data bits")
        }

        ComboBox {
            id: dataBitsCombo
            model: [5, 6, 7, 8]
        }

        ///////////////////  Flow control ///////////////////
        Label {
            text: qsTr("Flow control")
        }

        ComboBox {
            id: flowControlCombo
            textRole: "key"
            model: ListModel {
                ListElement {
                    key: "None";
                    value: 0
                }
                ListElement {
                    key: "Hardware";
                    value: 1
                }
                ListElement {
                    key: "Software";
                    value: 2
                }
            }
        }

        ///////////////////  Stop bits ///////////////////
        Label {
            text: qsTr("Stop bits")
        }

        ComboBox {
            id: stopBitsCombo
            model: [1,2]
        }

        ///////////////////  Direction ///////////////////
        Label {
            text: qsTr("Open for")
        }

        ComboBox {
            id: directionCombo

            textRole: "key"
            model: ListModel {
                ListElement {
                    key: "Read";
                    value: 1
                }
                ListElement {
                    key: "Write";
                    value: 2
                }
                ListElement {
                    key: "ReadWrite";
                    value: 3
                }
            }
        }
    }

    Button {
        id : connectButton
        text: qsTr("Connect")
        anchors.topMargin: settingLayout.rowSpacing
        anchors.top: settingLayout.bottom
        anchors.right: settingLayout.right
    }

    function setLastUsedPort() {
        var indexFound = portsCombo.find(SerialPortManager.lastUsedPort, Qt.MatchExactly)
        if(indexFound !== -1)
            portsCombo.currentIndex = indexFound
    }

    Component.onCompleted: {
        SerialPortManager.portsChanged.connect(setLastUsedPort)
    }
}
