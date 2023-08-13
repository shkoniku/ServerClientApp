#ifndef DBCREATOR_H
#define DBCREATOR_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>

class DbCreator
{
public:
    DbCreator();
    QSqlQueryModel queryModel;
    void InsertDataToBlocks(QString &id, QString &Name, QString &IP, QString &BoardCount,
                            QString &MtR, QString &MtC, QString &Description,
                            QString &Label);
    void InsertDataToBoards(QString &blockId, QString &id, QString &Num, QString &Name,
                            QString &PortCount, QString &IntLinks, QString &Algoritms);
    void InsertDataToPorts(QString &boardId, QString &id,
                           QString &Num, QString &Media, QString &Signal);
    void GetDataFrom(QString &name);
private:
    QSqlDatabase sqlitedb;

};

#endif // DBCREATOR_H
