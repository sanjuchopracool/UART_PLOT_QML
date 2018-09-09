#include "MainArea.h"
#include "ui_MainArea.h"
#include <QQuickView>
#include <QQmlContext>

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

    m_serial_port_manager = new SerialPortManager(this);
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
    setGeometry(m_setting.value(sGeometry).toRect());
}

MainArea::~MainArea()
{
    delete m_serial_port_manager;
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


void MainArea::closeEvent(QCloseEvent *event)
{
    m_setting.setValue(sGeometry, geometry());
    event->accept();
}
