#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QXmlStreamReader>
#include <QDir>
#include <QFileInfo>
#include <QtSql>
#include <QSqlDatabase>


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    QVector <QString*> FileNames;
    void SendtoClient(QString str);
    void InsertDataToBlocks(QString &id, QString &Name, QString &IP, QString &BoardCount,
                            QString &MtR, QString &MtC, QString &Description,
                            QString &Label);
    void InsertDataToBoards(QString &blockId, QString &id, QString &Num, QString &Name,
                            QString &PortCount, QString &IntLinks, QString &Algoritms);
    void InsertDataToPorts(QString &boardId, QString &id,
                           QString &Num, QString &Media, QString &Signal);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
