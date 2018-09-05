#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "SerialPortManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Sanjay Kumar");
    app.setOrganizationDomain("gnuchops.wordpress.com");
    app.setApplicationName("UART PLOT");

    QQmlApplicationEngine engine;

    SerialPortManager::register_serialport_types();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
