import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Window 2.3

Window {
    visible: true
    Material.theme: Material.Dark

    minimumWidth: setting.implicitWidth + 20
    minimumHeight: setting.implicitHeight + 20

    Page
    {
        anchors.fill: parent
        PortSettingItem {
            id: setting
            anchors.fill: parent
            anchors.margins: 10
        }
    }
}
