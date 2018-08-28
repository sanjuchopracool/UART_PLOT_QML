#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    m_refresh_ports_timer = new QTimer(this);
    connect(m_refresh_ports_timer, SIGNAL(timeout()), this, SLOT(checkPorts()));
    m_refresh_ports_timer->start(1000);

    //TODO: remove this code after test
    m_last_used_port = "Object5";
}

SerialPortManager::~SerialPortManager()
{
    m_refresh_ports_timer->stop();
}

void SerialPortManager::checkPorts()
{
    //TODO: remove this code after test
    static int counter = 1;
    m_ports.append(QString("Object%1").arg(counter++));
    emit portsChanged();
}

const QStringList &SerialPortManager::ports()
{
    return  m_ports;
}

const QString &SerialPortManager::lastUsedPort()
{
    return  m_last_used_port;
}
