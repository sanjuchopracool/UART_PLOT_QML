#include "MainArea.h"
#include "ui_MainArea.h"
#include <QQuickView>
#include <QQmlContext>
#include <QScrollBar>
#include <QDesktopWidget>

namespace
{
const QString sGeometry("Geometry");
}

MainArea::MainArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainArea),
    m_setting(this)
{
    ui->setupUi(this);
    ui->horizontalLayout->setMargin(0);
    ui->horizontalLayout->setSpacing(0);
    ui->plainTextEdit->setReadOnly(true);

    m_serial_port_manager = new SerialPortManager(this);
    m_serial_port_manager->loadFromSettings(m_setting);
    connect(m_serial_port_manager, SIGNAL(dataRead(QByteArray)), this, SLOT(onDataReceived(QByteArray)));

    QQuickView *view = new QQuickView();
    view->rootContext()->setContextProperty("SerialPortManager",
                                            m_serial_port_manager);
    view->setSource(QUrl("qrc:/main.qml"));
    m_view = QWidget::createWindowContainer(view, this);
    m_view->setFocusPolicy(Qt::TabFocus);

    ui->horizontalLayout->insertWidget(0, m_view);
    m_item = view->rootObject();
    m_view->setMinimumWidth(m_item->implicitWidth());
    connect(m_item, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));
    onStateChanged(m_item->property("state").toString());

    setMinimumHeight(300);
    setWindowTitle("UART PLOT");

    QRect defaultGeometry(0,0, 700, 400);
    defaultGeometry.moveCenter(QDesktopWidget().screenGeometry().center());
    setGeometry(m_setting.value(sGeometry, defaultGeometry).toRect());

    onConnectionChanged();

    // Connections
    connect(m_serial_port_manager,SIGNAL(connectedChanged()),
            this, SLOT(onConnectionChanged()));
    ui->lineEdit->setPlaceholderText(tr("Send data here!!"));
}

MainArea::~MainArea()
{
    delete ui;
}

void MainArea::onStateChanged(const QString &state)
{
    if("show"==state)
    {
        m_view->setFixedWidth(m_item->implicitWidth() +
                              m_item->property("settingPageWidth").toInt());
    }
    else
    {
        m_view->setFixedWidth(m_item->implicitWidth());
    }
}

void MainArea::onDataReceived(const QByteArray &inData)
{
    ui->plainTextEdit->moveCursor(QTextCursor::End);
    ui->plainTextEdit->insertPlainText(inData);
}


void MainArea::closeEvent(QCloseEvent *event)
{
    m_setting.setValue(sGeometry, geometry());
    m_serial_port_manager->saveToSettings(m_setting);
    event->accept();
}

void MainArea::on_lineEdit_returnPressed()
{
    m_serial_port_manager->sendData(ui->lineEdit->text().toLatin1());
    ui->lineEdit->clear();
}

void MainArea::onConnectionChanged()
{
    bool visible = false;
    if(m_serial_port_manager->connected())
    {
        if(m_serial_port_manager->portSetting().direction & QSerialPort::WriteOnly)
            visible = true;
    }

    ui->lineEdit->setVisible(visible);
}
