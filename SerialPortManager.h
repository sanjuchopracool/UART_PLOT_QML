#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QQmlEngine>
#include <QtSerialPort>

QT_FORWARD_DECLARE_CLASS(QJSEngine)

struct PortSetting
{
    Q_GADGET
    Q_PROPERTY(int baudRate MEMBER baudRate)
    Q_PROPERTY(int dataBits MEMBER dataBits)
    Q_PROPERTY(int stopBits MEMBER stopBits)
    Q_PROPERTY(int parity MEMBER parity)
    Q_PROPERTY(int flowControl MEMBER flowControl)
    Q_PROPERTY(int direction MEMBER direction)
public:
    int baudRate = QSerialPort::Baud9600;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int parity = QSerialPort::NoParity;
    int flowControl = QSerialPort::NoFlowControl;
    int direction = QSerialPort::Input | QSerialPort::Output;
};

Q_DECLARE_METATYPE(PortSetting)

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(const QString& lastUsedPort READ lastUsedPort)
    Q_PROPERTY(const PortSetting& portSetting READ portSetting)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    static void register_serialport_types() {
        // Third, register the singleton type provider with QML by calling this function in an initialization function.
        qmlRegisterSingletonType<SerialPortManager>("com.sanjay.serial", 1, 0, "SerialPortManager",
                                                    [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject  * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)

            SerialPortManager *manager = new SerialPortManager;
            return manager;
        });
        qmlRegisterUncreatableType<PortSetting>("com.sanjay.serial", 1, 0, "PortSetting", "can not create PortSetting type object");
    }

signals:
    void portsChanged();
    void connectedChanged();
    void dataRead(QByteArray data);

public slots:
    void connectToPort(const QString &inPortName, const PortSetting &inPortSetting = PortSetting());
    void disconnect();

private slots:
    void checkPorts();
    void readData();

private:
    const QStringList &ports()const;
    const QString &lastUsedPort() const;
    const PortSetting &portSetting() const;
    bool connected() const;

private:
    QStringList     m_ports;
    QString         m_last_used_port;
    QTimer          *m_refresh_device_timer = new QTimer(this);
    PortSetting     m_port_setting;
    QSerialPort*    m_port = new QSerialPort(this);
    bool            m_connected = false;
};

#endif // SERIALPORTMANAGER_H
