#ifndef MAINAREA_H
#define MAINAREA_H

#include <QWidget>
#include <QQuickItem>
#include <QSettings>
#include "SerialPortManager.h"

namespace Ui {
class MainArea;
}

class MainArea : public QWidget
{
    Q_OBJECT

public:
    explicit MainArea(QWidget *parent = nullptr);
    ~MainArea();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onStateChanged(const QString& state);
private:
    Ui::MainArea    *ui;
    QWidget         *m_view = nullptr;
    QQuickItem      *m_item;
    QSettings       m_setting;
    SerialPortManager *m_serial_port_manager = nullptr;
};

#endif // MAINAREA_H
