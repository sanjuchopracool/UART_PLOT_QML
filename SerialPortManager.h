#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QQmlEngine>

QT_FORWARD_DECLARE_CLASS(QJSEngine)

struct PortSetting
{
    Q_GADGET
    Q_PROPERTY(int baudRate MEMBER baudRate)
    Q_PROPERTY(int dataBits MEMBER dataBits)
    Q_PROPERTY(int stopBits MEMBER stopBits)
public:
    int baudRate;
    int dataBits;
    int stopBits;
};

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(const QString& lastUsedPort READ lastUsedPort)

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

signals:
    void portsChanged();

private slots:
    void checkPorts();

private:
    const QStringList &ports();
    const QString &lastUsedPort();

private:
    QStringList     m_ports;
    QString         m_last_used_port;
    QTimer          *m_refresh_ports_timer;
};

static QObject *serialport_manager_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
  {
      Q_UNUSED(engine)
      Q_UNUSED(scriptEngine)

      SerialPortManager *manager = new SerialPortManager;
      return manager;
  }

static void register_serialport_manager_as_singleton()
{
    // Third, register the singleton type provider with QML by calling this function in an initialization function.
    qmlRegisterSingletonType<SerialPortManager>("com.sanjay.serial", 1, 0, "SerialPortManager", serialport_manager_provider);
}

#endif // SERIALPORTMANAGER_H
