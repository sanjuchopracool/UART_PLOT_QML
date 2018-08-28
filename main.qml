import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ApplicationWindow {
    Material.theme: Material.Dark
    visible: true
    title: qsTr("Hello World")

    minimumWidth: setting.implicitWidth + 20
    minimumHeight: setting.implicitHeight + 20
    PortSettingItem {
        id: setting
        anchors.fill: parent
    }
}
