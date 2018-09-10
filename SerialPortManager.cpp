#include "SerialPortManager.h"
#include <qserialportinfo.h>

namespace
{
const QString cSerialPortManager("SerialPortManager");
const QString cLastUsedPort("Port");
const QString cConnectionStatus("Connected");
}

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    connect(m_refresh_device_timer, SIGNAL(timeout()), this, SLOT(checkPorts()));
    connect(m_port, &QSerialPort::readyRead, this, &SerialPortManager::readData);

    checkPorts();
    m_refresh_device_timer->start(1000);
}

SerialPortManager::~SerialPortManager()
{
    m_port->close();
    m_refresh_device_timer->stop();
}

void SerialPortManager::saveToSettings(QSettings &setting)
{
    setting.beginGroup(cSerialPortManager);
    setting.setValue(cLastUsedPort, m_last_used_port);
    setting.setValue(cConnectionStatus, m_connected);
    setting.endGroup();
}

void SerialPortManager::loadFromSettings(QSettings &setting)
{
    setting.beginGroup(cSerialPortManager);
    m_last_used_port = setting.value(cLastUsedPort).toString();
    m_connected = setting.value(cConnectionStatus).toBool();
    setting.endGroup();

    if(m_connected)
        tryToConnect();
}

void SerialPortManager::checkPorts()
{
    QStringList ports;
    for( auto const& info : QSerialPortInfo::availablePorts())
    {
        ports << info.portName();
    }

    if(ports != m_ports)
    {
        m_ports = ports;
        portsChanged();
    }
}

void SerialPortManager::readData()
{
    emit dataRead(m_port->readAll());
}

void SerialPortManager::connectToPort(const QString &inPort, const PortSetting &inPortSetting)
{
    m_last_used_port = inPort;
    m_port_setting = inPortSetting;

    connectInternal();
}

void SerialPortManager::disconnect()
{
    if(m_port->isOpen())
    {
        m_port->close();
        if(m_connected)
        {
            m_connected = false;
            emit connectedChanged();
            m_refresh_device_timer->start();
        }
    }
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

bool SerialPortManager::connected() const
{
    return m_connected;
}

void SerialPortManager::tryToConnect()
{
    if(m_last_used_port.size())
    {
        if(m_ports.contains(m_last_used_port))
        {
            connectInternal();
        }
    }
}

void SerialPortManager::connectInternal()
{
    m_port->close();
    m_port->setPortName(m_last_used_port);
    m_port->setBaudRate(m_port_setting.baudRate);
    m_port->setDataBits(static_cast<QSerialPort::DataBits>(m_port_setting.dataBits));
    m_port->setStopBits(static_cast<QSerialPort::StopBits>(m_port_setting.stopBits));
    m_port->setParity(static_cast<QSerialPort::Parity>(m_port_setting.parity));
    m_port->setFlowControl(static_cast<QSerialPort::FlowControl>(m_port_setting.flowControl));

    if(m_port->open(static_cast<QSerialPort::OpenMode>(m_port_setting.direction)))
    {
        m_connected = true;
        emit connectedChanged();
        m_refresh_device_timer->stop();
    }
    else
    {
        m_connected = false;
    }
}
