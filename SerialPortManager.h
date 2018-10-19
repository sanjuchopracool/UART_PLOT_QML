#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QQmlEngine>
#include <QtSerialPort>
#include <QSettings>

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

    void saveToSettings(QSettings &setting);
    void loadFromSettings(QSettings &setting);
};

Q_DECLARE_METATYPE(PortSetting)

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(const QString& lastUsedPort READ lastUsedPort)
    Q_PROPERTY(PortSetting portSetting READ portSetting WRITE setPortSetting)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    static void register_serialport_types() {
        qmlRegisterUncreatableType<PortSetting>("com.sanjay.serial", 1, 0, "PortSetting", "can not create PortSetting type object");
    }

    void saveToSettings(QSettings& setting);
    void loadFromSettings(QSettings &setting);

signals:
    void portsChanged();
    void connectedChanged();
    void dataRead(QByteArray data);

public slots:
    void connectToPort(const QString &inPortName, const PortSetting &inPortSetting = PortSetting());
    void disconnect();

private slots:
    void checkPortsAndConnect();
    void readData();
    void onErrorOccurred(QSerialPort::SerialPortError error);

private:
    const QStringList &ports()const;
    const QString &lastUsedPort() const;
    PortSetting portSetting() const;
    void setPortSetting(const PortSetting& inSetting);
    bool connected() const;

    void tryToConnect();
    void connectInternal();

private:
    QStringList     m_ports;
    QString         m_last_used_port;
    QTimer          *m_refresh_device_timer = new QTimer(this);
    PortSetting     m_port_setting;
    QSerialPort*    m_port = new QSerialPort(this);
    bool            m_connected = false;
    bool            m_autoConnect = false;
};

#endif // SERIALPORTMANAGER_H
