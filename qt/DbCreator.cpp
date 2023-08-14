#include "DbCreator.h"

DbCreator::DbCreator()
{
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
}


void DbCreator::InsertDataToBlocks(QString &id, QString &Name, QString &IP, QString &BoardCount,
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
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?);");
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


void DbCreator::InsertDataToBoards(QString &blockId, QString &id, QString &Num, QString &Name,
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
                  "VALUES (?, ?, ?, ?, ?, ?, ?);");
    query.addBindValue(blockId);
    query.addBindValue(id);
    query.addBindValue(Num);
    query.addBindValue(Name);
    query.addBindValue(PortCount);
    query.addBindValue(IntLinks);
    query.addBindValue(Algoritms);
    if (!query.exec()) qDebug() << "error adding values to table: boards. id = " << id;
}


void DbCreator::InsertDataToPorts(QString &boardId, QString &id, QString &Num, QString &Media, QString &Signal)
{
    QSqlQuery query;
    query.prepare("INSERT INTO ports("
                  "boardId, "
                  "id, "
                  "Num, "
                  "Media, "
                  "Signal)"
                  "VALUES (?, ?, ?, ?, ?);");
    query.addBindValue(boardId);
    query.addBindValue(id);
    query.addBindValue(Num);
    query.addBindValue(Media);
    query.addBindValue(Signal);
    if (!query.exec()) qDebug() << "error adding values to table: ports. id = " << id;
}

void DbCreator::GetDataFrom(QString &name)
{
    queryModel.setQuery("SELECT * FROM "+name);
    if (queryModel.lastError().isValid())
    {
        qDebug() << "error selecting from table: " << name << " " << queryModel.lastError();
    }
    else
    {
        while (queryModel.canFetchMore())
            queryModel.fetchMore();
    }

}
