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
    QSqlDatabase sqlitedb = QSqlDatabase::addDatabase("QSQLITE");
    sqlitedb.setDatabaseName("../qt/blocks.sqlite");
    if (!sqlitedb.open())
    {
        qDebug() << "error opening database";
    }
    QString queryTable = "CREATE TABLE blocks("
                    "id TEXT PRIMARY KEY, "
                    "Name TEXT, "
                    "IP TEXT, "
                    "BoardCount TEXT, "
                    "MtR TEXT, "
                    "MtC TEXT, "
                    "Description TEXT, "
                    "Label TEXT);";
    QSqlQuery query;
    if (!query.exec(queryTable)) qDebug() << "error creating table: blocks";
    queryTable = "CREATE TABLE boards("
                 "blockId TEXT, "
                 "id TEXT PRIMARY KEY, "
                 "Num TEXT, "
                 "Name TEXT, "
                 "PortCount TEXT, "
                 "IntLinks TEXT, "
                 "Algoritms TEXT);";
    if (!query.exec(queryTable)) qDebug() << "error creating table: boards";
    queryTable = "CREATE TABLE ports("
                 "boardId TEXT, "
                 "id TEXT PRIMARY KEY, "
                 "Num TEXT, "
                 "Media TEXT, "
                 "Signal TEXT);";
    if (!query.exec(queryTable)) qDebug() << "error creating table: ports";
    QDir directory("../qt/XMLS");
    QStringList files = directory.entryList(QStringList() << "*.xml", QDir::Files);
    foreach (QString nameFile, files) {
        QFile* file = new QFile("../qt/XMLS/"+nameFile);
        qDebug() << file->fileName();
        QString blockId = "0", boardId = "0";
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

                    if (xml.name() == "block")
                    {
                        QString attributs[8] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            if (iterator == 0)
                            {
                                blockId = xml.attributes()[0].value().toString();
                            }
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        InsertDataToBlocks(attributs[0], attributs[1], attributs[2],
                                            attributs[3], attributs[4], attributs[5],
                                            attributs[6], attributs[7]);
                    }

                    if (xml.name() == "board")
                    {
                        QString attributs[6] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            if (iterator == 0)
                            {
                                boardId = xml.attributes()[0].value().toString();
                            }
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        InsertDataToBoards(blockId, attributs[0], attributs[1], attributs[2],
                                            attributs[3],  attributs[4],  attributs[5]);
                    }

                    if (xml.name() == "port")
                    {
                        QString attributs[4] = {NULL};
                        for (int iterator = 0; iterator < xml.attributes().size(); iterator++)
                        {
                            attributs[iterator] = xml.attributes()[iterator].value().toString();
                        }
                        InsertDataToPorts(boardId, attributs[0], attributs[1],
                                          attributs[2], attributs[3]);
                    }
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


void Server::InsertDataToBlocks(QString &id, QString &Name, QString &IP, QString &BoardCount,
                                QString &MtR, QString &MtC, QString &Description,
                                QString &Label)
{
    QSqlQuery query;
    query.prepare("INSERT INTO blocks("
                  "id, "
                  "Name, "
                  "IP, "
                  "BoardCount, "
                  "MtR, "
                  "Mtc, "
                  "Description, "
                  "Label) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(Name);
    query.addBindValue(IP);
    query.addBindValue(BoardCount);
    query.addBindValue(MtR);
    query.addBindValue(MtC);
    query.addBindValue(Description);
    query.addBindValue(Label);
    if (!query.exec()) qDebug() << "error adding values to table: blocks id = " << id;

}


void Server::InsertDataToBoards(QString &blockId, QString &id, QString &Num, QString &Name,
                                QString &PortCount, QString &IntLinks, QString &Algoritms)
{
    QSqlQuery query;
    query.prepare("INSERT INTO boards("
                  "blockId, "
                  "id, "
                  "Num, "
                  "Name, "
                  "PortCount, "
                  "IntLinks, "
                  "Algoritms) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(blockId);
    query.addBindValue(id);
    query.addBindValue(Num);
    query.addBindValue(Name);
    query.addBindValue(PortCount);
    query.addBindValue(IntLinks);
    query.addBindValue(Algoritms);
    if (!query.exec()) qDebug() << "error adding values to table: boards. id = " << id;
}


void Server::InsertDataToPorts(QString &boardId, QString &id, QString &Num, QString &Media, QString &Signal)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ports("
                  "boardId, "
                  "id, "
                  "Num, "
                  "Media, "
                  "Signal)"
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(boardId);
    query.addBindValue(id);
    query.addBindValue(Num);
    query.addBindValue(Media);
    query.addBindValue(Signal);
    if (!query.exec()) qDebug() << "error adding values to table: ports. id = " << id;
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
