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
    QDir directory("../qt/XMLS");
    QStringList files = directory.entryList(QStringList() << "*.xml", QDir::Files);
    foreach (QString nameFile, files) {
        QFile* file = new QFile("../qt/XMLS/"+nameFile);
        qDebug() << file->fileName();
        if (file->open(QIODevice::ReadOnly | QIODevice::Text) && QFileInfo::exists("../qt/XMLS/"+nameFile) && QFileInfo("../qt/XMLS/"+nameFile).isFile())
        {
            qDebug() << "ok";
            QXmlStreamReader xml(file);
            while (!xml.atEnd() && !xml.hasError())
            {
                QXmlStreamReader::TokenType token = xml.readNext();
                if (token == QXmlStreamReader::StartDocument)
                {
                    continue;
                }
                if (token == QXmlStreamReader::StartElement)
                {
                    //if (xml.name() == "")
                }
            }
        }
        else
        {
            qDebug() << "cant open file";
        }
        file->close();
    }

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
