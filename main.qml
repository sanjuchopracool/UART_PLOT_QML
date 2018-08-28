import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import com.sanjay.serial 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ComboBox {
        id: ports
        model: SerialPortManager.ports
    }

    function setLastUsedPort() {
        ports.currentIndex = ports.find(SerialPortManager.lastUsedPort, Qt.MatchExactly)
    }

    Component.onCompleted: {
        SerialPortManager.portsChanged.connect(setLastUsedPort)
    }
}
