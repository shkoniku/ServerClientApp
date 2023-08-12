#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);
    qDebug() << "trying get to server";
    if (in.status() == QDataStream::Ok)
    {
        ui->textBrowser->append("server is online");
        QString str;
        in >> str;
        ui->textBrowser->append(str);
    }
    else
    {
        qDebug() << "server is offline";
        ui->textBrowser->append("read error");
    }
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 8000);
    SendToServer("get");
    ui->textBrowser->append("start");
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << str;
    socket->write(Data);
}
