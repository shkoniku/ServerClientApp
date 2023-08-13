#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QSqlQuery>
#include "xmlParser.h"
#include "DbCreator.h"


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    Parser parser;
    DbCreator creator;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    QVector <QString*> FileNames;
    void SendtoClient(QTcpSocket *socket, QString str);
    void PrepareData(QTcpSocket *socket);


public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
