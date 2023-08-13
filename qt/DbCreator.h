#ifndef DBCREATOR_H
#define DBCREATOR_H

#include <QtSql>
#include <QSqlDatabase>

class DbCreator
{
public:
    DbCreator();
    void InsertDataToBlocks(QString &id, QString &Name, QString &IP, QString &BoardCount,
                            QString &MtR, QString &MtC, QString &Description,
                            QString &Label);
    void InsertDataToBoards(QString &blockId, QString &id, QString &Num, QString &Name,
                            QString &PortCount, QString &IntLinks, QString &Algoritms);
    void InsertDataToPorts(QString &boardId, QString &id,
                           QString &Num, QString &Media, QString &Signal);
private:
    QSqlDatabase sqlitedb;

};

#endif // DBCREATOR_H
