#include "SerialPortManager.h"
#include <qserialportinfo.h>

namespace
{
const QString cSerialPortManager("SerialPortManager");
const QString cLastUsedPort("Port");
const QString cConnectionStatus("Connected");

const QString cBaudRate("BaudRate");
const QString cDataBits("DataBits");
const QString cStopBits("StopBits");
const QString cParity("Parity");
const QString cFlowControl("FlowControl");
const QString cDirection("Direction");
}

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    connect(m_refresh_device_timer, SIGNAL(timeout()), this, SLOT(checkPortsAndConnect()));
    connect(m_port, &QSerialPort::readyRead, this, &SerialPortManager::readData);
    connect(m_port, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(onErrorOccurred(QSerialPort::SerialPortError)));

    checkPortsAndConnect();
    m_refresh_device_timer->start(1000);
}

SerialPortManager::~SerialPortManager()
{
    m_refresh_device_timer->stop();

    if(m_port->isOpen())
        m_port->close();
}

void SerialPortManager::saveToSettings(QSettings &setting)
{
    setting.beginGroup(cSerialPortManager);
    setting.setValue(cLastUsedPort, m_last_used_port);
    setting.setValue(cConnectionStatus, m_connected);
    m_port_setting.saveToSettings(setting);
    setting.endGroup();
}

void SerialPortManager::loadFromSettings(QSettings &setting)
{
    setting.beginGroup(cSerialPortManager);
    m_last_used_port = setting.value(cLastUsedPort).toString();
    bool wasConnected = setting.value(cConnectionStatus).toBool();
    m_port_setting.loadFromSettings(setting);
    setting.endGroup();

    if(wasConnected)
    {
        tryToConnect();

        if (!m_connected)
            m_autoConnect = true;
    }
}

void SerialPortManager::checkPortsAndConnect()
{
    QStringList ports;
    for( auto const& info : QSerialPortInfo::availablePorts())
    {
        ports << info.portName();
    }

    if(ports != m_ports)
    {
        m_ports = ports;
        emit portsChanged();
    }

    if(m_autoConnect)
        tryToConnect();
}

void SerialPortManager::readData()
{
    emit dataRead(m_port->readAll());
}

void SerialPortManager::onErrorOccurred(QSerialPort::SerialPortError error)
{
    switch (error) {
    case QSerialPort::ResourceError:
        disconnect();
        m_autoConnect = true;
        break;
    default:
        break;
    }
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

PortSetting SerialPortManager::portSetting() const
{
    return  m_port_setting;
}

void SerialPortManager::sendData(const QByteArray &inData)
{
    if(m_port->isOpen())
    {
        m_port->write(inData);
    }
}

void SerialPortManager::setPortSetting(const PortSetting &inSetting)
{
    m_port_setting = inSetting;
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
    if(m_port->isOpen())
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
        m_autoConnect = false;
    }
    else
    {
        m_connected = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// PortSetting
///////////////////////////////////////////////////////////////////////////////

void PortSetting::saveToSettings(QSettings &setting)
{
    setting.setValue(cBaudRate, baudRate);
    setting.setValue(cDataBits, dataBits);
    setting.setValue(cStopBits, stopBits);
    setting.setValue(cParity, parity);
    setting.setValue(cFlowControl, flowControl);
    setting.setValue(cDirection, direction);
}

void PortSetting::loadFromSettings(QSettings &setting)
{
    baudRate = setting.value(cBaudRate, baudRate).toInt();
    dataBits = setting.value(cDataBits, dataBits).toInt();
    stopBits = setting.value(cStopBits, stopBits).toInt();
    parity = setting.value(cParity, parity).toInt();
    flowControl = setting.value(cFlowControl, flowControl).toInt();
    direction = setting.value(cDirection, direction).toInt();
}
