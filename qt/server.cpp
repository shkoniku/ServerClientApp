#include "server.h"
#include "xmlParser.h"
#include "DbCreator.h"

Server::Server()
{
    if (this->listen(QHostAddress::Any, 8000))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
    Parser parser;
    DbCreator creator;
    parser.Parse(creator);
}


void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket -> setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client " << socketDescriptor << " connected";
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);
    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
        QString str;
        in >> str;
        qDebug() << str;
        SendtoClient("put");
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Server::SendtoClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << str;
    for(int iterator = 0; iterator < Sockets.size(); iterator++)
    {
        Sockets[iterator]->write(Data);
    }
}
