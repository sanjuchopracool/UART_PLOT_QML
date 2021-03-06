import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.sanjay.serial 1.0
import Qt.labs.settings 1.0

Item {
    id: mainItem

    property int extraWidth: 0
    implicitWidth: settingLayout.implicitWidth + extraWidth
    implicitHeight: settingLayout.implicitHeight + connectButton.implicitHeight + settingLayout.rowSpacing

    Rectangle {
        anchors.fill: parent
        anchors.margins: -10
        border.width: 1

        MouseArea {
            id: dragArea
            width: 10
            height: 10

            cursorShape: Qt.SizeFDiagCursor
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            hoverEnabled: true


            drag {
                target: mainItem
                axis: Drag.XAxis
            }

            onMouseXChanged: {
                if(drag.active){

                    if( mainItem.extraWidth + mouseX > 0 )
                        mainItem.extraWidth += mouseX
                }
            }
        }
    }


    GridLayout
    {
        id: settingLayout
        columns: 4
        anchors.fill: parent
        /////////////////// Ports ///////////////////
        Label {
            text: qsTr("Port")
        }

        ComboBox {
            id: portsCombo
            model: SerialPortManager.ports
            Layout.fillWidth: true
            Layout.columnSpan: 3

            onCountChanged: {
                connectButton.enabled = count
            }
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
            Layout.fillWidth: true

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
            Layout.fillWidth: true

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
            Layout.fillWidth: true

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

        RowLayout {
            Layout.row: 4
            Layout.column: 3
            Button {
                Layout.fillWidth: true
                id : connectButton
                text: qsTr("Connect")
                enabled: false

                onClicked: {
                    if(SerialPortManager.connected)
                        SerialPortManager.disconnect()
                    else
                    {
                        var newSetting = SerialPortManager.portSetting
                        newSetting.baudRate = baudRateCombo.model[baudRateCombo.currentIndex]
                        newSetting.dataBits = dataBitsCombo.model[dataBitsCombo.currentIndex]
                        newSetting.stopBits = stopBitsCombo.model[stopBitsCombo.currentIndex]

                        newSetting.parity = parityCombo.model.get(parityCombo.currentIndex).value
                        newSetting.flowControl = flowControlCombo.model.get(flowControlCombo.currentIndex).value
                        newSetting.direction = directionCombo.model.get(directionCombo.currentIndex).value

                        SerialPortManager.connectToPort(portsCombo.currentText, newSetting)
                    }
                }
            }
        }
    }

    function setLastUsedPort() {
        var indexFound = portsCombo.find(SerialPortManager.lastUsedPort, Qt.MatchExactly)
        if(indexFound !== -1)
            portsCombo.currentIndex = indexFound
    }

    function setCurrentIndexForComboByText(combobox, value) {
        combobox.currentIndex = combobox.find(value, Qt.MatchExactly)
    }

    function setCurrentIndexForComboByValue(combobox, value) {
        var count = combobox.count;
        var model = combobox.model
        for(var i =0; i < count; ++i) {
            if(value === model.get(i).value) {
                combobox.currentIndex = i
                break
            }
        }
    }
    function enableUI(enabled) {
        portsCombo.enabled = enabled
        baudRateCombo.enabled = enabled;
        dataBitsCombo.enabled = enabled;
        stopBitsCombo.enabled = enabled;
        parityCombo.enabled = enabled;
        flowControlCombo.enabled = enabled;
        directionCombo.enabled = enabled;
    }

    function onConnectionChanged() {
        if(SerialPortManager.connected)
        {
            enableUI(false)
            connectButton.text = qsTr("Disconnect")
        }
        else
        {
            enableUI(true)
            connectButton.text = qsTr("Connect")
        }
    }

    Settings {
        property alias extraWidth: mainItem.extraWidth
    }

    Component.onCompleted: {
        setLastUsedPort()
        onConnectionChanged()
        var portSetting = SerialPortManager.portSetting
        setCurrentIndexForComboByText(baudRateCombo, portSetting.baudRate)
        setCurrentIndexForComboByText(dataBitsCombo, portSetting.dataBits)
        setCurrentIndexForComboByText(stopBitsCombo, portSetting.stopBits)

        setCurrentIndexForComboByValue(parityCombo, portSetting.parity)
        setCurrentIndexForComboByValue(flowControlCombo, portSetting.flowControl)
        setCurrentIndexForComboByValue(directionCombo, portSetting.direction)

        SerialPortManager.portsChanged.connect(setLastUsedPort)
        SerialPortManager.connectedChanged.connect(onConnectionChanged)
    }
}
