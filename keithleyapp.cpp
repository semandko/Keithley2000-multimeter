#include "keithleyapp.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

// Конструктор класу KeithleyApp
KeithleyApp::KeithleyApp(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    counter(0)
{
    ui->setupUi(this);
    
    serialPort = new QSerialPort(this);

    setupConnections();

    populateSerialPorts();
    

    file.setFileName("data.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QMessageBox::critical(this, "Error", "Cannot to open the file!");
    }
    else
    {
        stream.setDevice(&file);
        stream << "№;Time;Voltage\n";
    }
}

KeithleyApp::~KeithleyApp()
{
    if (file.isOpen())
    {
        file.close();
    }
    
    delete ui;
    
    if (serialPort)
    {
        if (serialPort->isOpen())
        {
            serialPort->close();
        }
        delete serialPort;
    }
}

void KeithleyApp::setupConnections()
{
    connect(ui->refreshButton, &QPushButton::clicked, this, &KeithleyApp::populateSerialPorts);
    connect(ui->connectButton, &QPushButton::clicked, this, &KeithleyApp::connectToPort);
    connect(ui->startMeasurementButton, &QPushButton::clicked, this, &KeithleyApp::doMeasurement);
}

void KeithleyApp::populateSerialPorts()
{
    ui->portComboBox->clear();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
    {
        ui->portComboBox->addItem(info.portName());
    }
}

void KeithleyApp::connectToPort()
{
    serialPort->setPortName(ui->portComboBox->currentText());
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, "З'єднання", "Успішно під'єднано до порту " + ui->portComboBox->currentText());
        ui->connectButton->setEnabled(false);
        ui->startMeasurementButton->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(this, "Помилка", "Не вдалося відкрити порт");
    }
}

void KeithleyApp::doMeasurement()
{
    if (!serialPort->isOpen())
    {
        QMessageBox::warning(this, "Помилка", "Порт не відкритий!");
        return;
    }

    QByteArray cmd = ":MEAS:VOLT:DC?\r\n";
    serialPort->write(cmd);
    serialPort->flush();

    if (serialPort->waitForReadyRead(2000))
    {
        QByteArray response = serialPort->readAll();
        while (serialPort->waitForReadyRead(100))
        {
            response += serialPort->readAll();
        }

        counter++;
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString voltageStr = QString(response).trimmed();
        
        // Оновлення UI
        ui->resultLabel->setText("Результат: " + voltageStr + " В");

        QString line = QString("%1;%2;%3\n")
                           .arg(counter)
                           .arg(timeStr)
                           .arg(voltageStr);

        stream << line;
        stream.flush();

    }
    else
    {
        QMessageBox::warning(this, "Помилка", "Таймаут відповіді");
    }
}
