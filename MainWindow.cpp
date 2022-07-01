#include "MainWindow.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include "Platform_Types.h"
#include "inc/Com.h"

Ui::MainWindowClass* ui_extern;
extern uint8 ComIPdu0Buffer[2];
extern uint8 ComIPdu1Buffer[2];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui_extern = &ui;
    ui_extern->setupUi(this);
    ui.setupUi(this);
    _server.listen(QHostAddress::Any, 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    ui.larrow_b->setVisible(0);
    ui.rarrow_b->setVisible(0);
    ui.fuel_b->setVisible(0);
    ui.battery_b->setVisible(0);
    ui.opendoor_b->setVisible(0);
    ui.light_b->setVisible(0);
    ui.fix_b->setVisible(0);
    
}

MainWindow::~MainWindow()
{}
void MainWindow::onNewConnection()
{
    QTcpSocket* clientSocket = _server.nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    for (QTcpSocket* socket : _sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void MainWindow::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();
    ui.statusbar->showMessage(datas);

    QByteArray ipdu_id = datas.mid(0, 1);
    QByteArray data = datas.mid(1);
    /// <summary>
    ///  QByte to array
    /// </summary>
    /// com_receive_signal(signal_id,ptr to data)
    /// 
    QString DataAsString = QString(data);
    uint8 signal = 0;

    if (DataAsString[0] == 'k');

    // choose the ipdu depend on the data
    else if (ipdu_id == "0") {
        memcpy(ComIPdu0Buffer, data.data(), data.size());

        if (Com_CheckUpdatedbit(0)) {
            Com_ReceiveSignal(0, &signal);
            ui.fuel_b->setVisible((signal));
        }
        else if (Com_CheckUpdatedbit(1)) {

            Com_ReceiveSignal(1, &signal);
            ui.opendoor_b->setVisible((signal));

            //ui.battery_b->setVisible(DataAsString.mid(1).toInt());
        }
        else if (Com_CheckUpdatedbit(2)) {

            Com_ReceiveSignal(2, &signal);
            ui.light_b->setVisible((signal));

            //ui.light_b->setVisible(DataAsString.mid(1).toInt());
        }
        else if (Com_CheckUpdatedbit(3)) {

            Com_ReceiveSignal(3, &signal);
            ui.battery_b->setVisible((signal));

            //ui.opendoor_b->setVisible(DataAsString.mid(1).toInt());
        }
        else if (Com_CheckUpdatedbit(4)) {

            Com_ReceiveSignal(4, &signal);
            ui.fix_b->setVisible((signal));

            //ui.fuel_b->setVisible(DataAsString.mid(1).toInt());
        }
        else if (Com_CheckUpdatedbit(5))
        {
            Com_ReceiveSignal(5, &signal);
            ui.larrow_b->setVisible((signal));
            //ui.rarrow_b->setVisible(DataAsString.mid(1).toInt());
        }
        else if (Com_CheckUpdatedbit(6))
        {
            Com_ReceiveSignal(6, &signal);
            ui.rarrow_b->setVisible((signal));
            //ui.larrow_b->setVisible(DataAsString.mid(1).toInt());
        }

    }

    else if (ipdu_id == "1") {
        memcpy(ComIPdu1Buffer, data.data(), data.size());
        if (Com_CheckUpdatedbit(7)) {
            Com_ReceiveSignal(7, &signal);
            ui.speed->setText(QString::number(signal));
        }

        else if (Com_CheckUpdatedbit(8)) {
            Com_ReceiveSignal(8, &signal);
            ui.degree->setText(QString::number(signal));
        }
    }

    for (QTcpSocket* socket : _sockets) {
        if (socket != sender)
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
    }
}

void MainWindow::on_larrow_c_stateChanged(int arg1)
{
    ui.larrow_b->setVisible(arg1);
}


void MainWindow::on_rarrow_c_stateChanged(int arg1)
{
    ui.rarrow_b->setVisible(arg1);
}


void MainWindow::on_fuel_c_stateChanged(int arg1)
{
    ui.fuel_b->setVisible(arg1);
}


void MainWindow::on_opendoor_c_stateChanged(int arg1)
{
    ui.opendoor_b->setVisible(arg1);
}


void MainWindow::on_light_c_stateChanged(int arg1)
{
    ui.light_b->setVisible(arg1);
}


void MainWindow::on_battery_c_stateChanged(int arg1)
{
    ui.battery_b->setVisible(arg1);
}


void MainWindow::on_fix_c_stateChanged(int arg1)
{
    ui.fix_b->setVisible(arg1);
}
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QString s = QString::number(value);
    ui.speed->setText(s);
}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    QString s = QString::number(value);
    ui.degree->setText(s);
}

