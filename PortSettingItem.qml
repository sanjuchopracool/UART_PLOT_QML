import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.sanjay.serial 1.0

Item {
    width: settingLayout.implicitWidth
    height: settingLayout.implicitHeight

    GridLayout
    {
        id: settingLayout
        columns: 2
        anchors.fill: parent
        anchors.margins: 10

        /////////////////// Ports ///////////////////
        Label {
            text: qsTr("Port")
        }

        ComboBox {
            id: portsCombo
            model: SerialPortManager.ports
            Layout.fillWidth: true
        }

        ///////////////////  Baud Rate ///////////////////
        Label {
            text: qsTr("Baud rate")
        }

        ComboBox {
            id: baudRateCombo
            model: [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
        }

        ///////////////////  Data bits ///////////////////
        Label {
            text: qsTr("Data bits")
        }

        ComboBox {
            id: dataBitsCombo
            model: [5, 6, 7, 8]
        }

        ///////////////////  Stop bits ///////////////////
        Label {
            text: qsTr("Stop bits")
        }

        ComboBox {
            id: stopBitsCombo
            model: [1,2]
        }
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
