#include "server.h"



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
        if (str == "get")
        {
            this->PrepareData(socket);
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Server::SendtoClient(QTcpSocket *socket, QString &str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << str;
    socket->write(Data);
}

void Server::SendtoClient(QTcpSocket *socket, QStringList &letter)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << letter;
    socket->write(Data);
}

void Server::PrepareData(QTcpSocket *socket)
{
    QStringList tables = {"blocks;", "boards;", "ports;"};
    QStringList message = {};
    foreach(QString element, tables)
    {
        creator.GetDataFrom(element);        
        int firstBorder = creator.queryModel.rowCount();
        for (int iterator = 0; iterator < firstBorder; iterator++)
        {
            QString row = element;
            int secondBorder = creator.queryModel.record(iterator).count();
            for (int step = 0; step < secondBorder; step++)
            {
                row += "/" + creator.queryModel.record(iterator).value(step).toString();
            }
            message.append(row);
        }
    }
    this->SendtoClient(socket, message);
}
