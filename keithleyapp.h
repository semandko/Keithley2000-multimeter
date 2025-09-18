#ifndef KEITHLEYAPP_H
#define KEITHLEYAPP_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class KeithleyApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit KeithleyApp(QWidget *parent = nullptr);
    ~KeithleyApp();

private slots:
    void populateSerialPorts();
    void connectToPort();
    void doMeasurement();

private:
    void setupConnections();

    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QFile file;
    QTextStream stream;
    int counter;
};

#endif // KEITHLEYAPP_H
