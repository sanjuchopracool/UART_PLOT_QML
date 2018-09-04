#include "SerialPortManager.h"
#include <qserialportinfo.h>

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    m_refresh_device_timer = new QTimer(this);
    connect(m_refresh_device_timer, SIGNAL(timeout()), this, SLOT(checkPorts()));
    checkPorts();
    m_refresh_device_timer->start(1000);
}

SerialPortManager::~SerialPortManager()
{
    m_refresh_device_timer->stop();
}

void SerialPortManager::checkPorts()
{
    m_ports.clear();
    for( auto const& info : QSerialPortInfo::availablePorts())
    {
        m_ports << info.portName();
    }
    portsChanged();
}

const QStringList &SerialPortManager::ports() const
{
    return  m_ports;
}

const QString &SerialPortManager::lastUsedPort() const
{
    return  m_last_used_port;
}

const PortSetting &SerialPortManager::portSetting() const
{
    return  m_port_setting;
}
