#include "MainWindow.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

Ui::MainWindowClass ui_extern;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui_extern = ui;
    ui_extern.setupUi(this);
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
    QString DataAsString = QString(datas);
    if (DataAsString[0] == 'o');

    else if (DataAsString[0] == 's')
        ui.speed->setText(DataAsString.mid(1));

    else if (DataAsString[0] == 'd')
        ui.degree->setText(DataAsString.mid(1));

    else if (DataAsString[0] == 'f')
        ui.fix_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'b')
        ui.battery_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'l')
        ui.light_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'p')
        ui.opendoor_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'u')
        ui.fuel_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'r')
        ui.rarrow_b->setVisible(DataAsString.mid(1).toInt());
    else if (DataAsString[0] == 'e')
        ui.larrow_b->setVisible(DataAsString.mid(1).toInt());

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

