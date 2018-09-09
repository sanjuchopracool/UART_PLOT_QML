#include <QApplication>
#include "MainArea.h"
#include <QQuickStyle>

#include "SerialPortManager.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setOrganizationName("Sanjay Kumar");
    app.setOrganizationDomain("gnuchops.wordpress.com");
    app.setApplicationName("UART PLOT");

    SerialPortManager::register_serialport_types();
    QQuickStyle::setStyle("Universal");

    MainArea w;
    w.show();

    return app.exec();
}
